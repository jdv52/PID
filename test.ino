#include "Motor.h"

#define EN1   5
#define PH1   4
#define ENCA1 13 // Yellow
#define ENCB1 11  // White

#define EN2   22
#define PH2   24
#define ENCA2 15  // Yellow 
#define ENCB2 14  // White

#define K_P   1
#define K_I   0
#define K_D   0.1

volatile int pos1 = 0;

volatile int lastEncoded;

long lastMillis = 0;

Motor m1;
Motor m2;

void readEnc1() {
  int phaseA = digitalRead(ENCA1);
  int phaseB = digitalRead(ENCB1);

  int currentEncoded = (phaseA << 1) | phaseB;
  int sum = (m1._lastEncCount << 2) | currentEncoded;

  // Serial.print(phaseA);
  // Serial.print(", ");
  // Serial.print(phaseB);
  // Serial.print(", ");
  // Serial.println(m1.encCount);

  switch(sum){
    case 0b0001:
    case 0b0111:
    case 0b1110:
    case 0b1000:
    case 0b0011:
    case 0b1100:
      m1.encCount--;
      break;
    case 0b0010:
    case 0b1011:
    case 0b1101:
    case 0b0100:
    case 0b1010:
    case 0b0110:
      m1.encCount++;
      break;
  }
  m1._lastEncCount = currentEncoded;
}


void readEnc2() {
  int phaseA = digitalRead(ENCA2);
  int phaseB = digitalRead(ENCB2);

  int currentEncoded = (phaseA << 1) | phaseB;
  int sum = (m2._lastEncCount << 2) | currentEncoded;

  switch(sum){
    case 0b0001:
    case 0b0111:
    case 0b1110:
    case 0b1000:

      m2.encCount--;
      break;
    case 0b0010:
    case 0b1011:
    case 0b1101:
    case 0b0100:

      m2.encCount++;
      break;
  }
  m2._lastEncCount = currentEncoded;
}

void setup() {
  // put your setup code here, to run once:
  // driveMotor(&m2);

  Serial.begin(9600);
  m1 = motor_init(EN1, PH1, ENCA1, ENCB1, readEnc1, K_P, K_I, K_D);
  // m2 = motor_init(EN2, PH2, ENCA2, ENCB2, readEnc2, K_P, K_I, K_D);
  setAngleTarget(&m1, 60);

}

void loop() {
  // put your main code here, to run repeatedly:
   calcPID(&m1);
   if (millis() - lastMillis > 5000)  {
    long currentTime = millis();
    m1.encCountTarget += 60;
    lastMillis = currentTime;
   }


  // Serial.println(m1.encCount);
  driveMotor(&m1);

   Serial.print("SetPoint: "); Serial.print(m1.encCountTarget);
   Serial.print(", ");
   Serial.print("PWM: ");Serial.print(m1.pwm);
   Serial.print(", ");
   Serial.print("Current Count: ");Serial.println(m1.encCount);

   //calcPID(&m2);
// }
}
