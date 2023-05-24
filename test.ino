#include "Motor.h"

#define EN1   19
#define PH1   21
#define ENCA1 14
#define ENCB1 15

#define K_P   1
#define K_I   0
#define K_D   0.1

volatile int pos1 = 0;

volatile int lastEncoded;

long lastMillis = 0;

Motor m1;

void readEnc1() {
  int phaseA = digitalRead(ENCA1);
  int phaseB = digitalRead(ENCB1);

  int currentEncoded = (phaseA << 1) | phaseB;
  int sum = (m1._lastEncCount << 2) | currentEncoded;

  switch(sum){
    case 0b0001:
    case 0b0111:
    case 0b1110:
    case 0b1000:
      m1.encCount--;
      break;
    case 0b0010:
    case 0b1011:
    case 0b1101:
    case 0b0100:
      m1.encCount++;
      break;
  }
  m1._lastEncCount = currentEncoded;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  m1 = motor_init(EN1, PH1, ENCA1, ENCB1, readEnc1, K_P, K_I, K_D);
  setAngleTarget(&m1, 60);
}

void loop() {
  // put your main code here, to run repeatedly:
  calcPID(&m1);
  driveMotor(&m1);
  if (millis() - lastMillis > 5000)  {
    long currentTime = millis();
    m1.encCountTarget += 60;
    lastMillis = currentTime;
  }
}
