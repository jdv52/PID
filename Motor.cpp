#include "Motor.h"
#include "arduino.h"
#include "PID.h"

Motor motor_init(int en, int ph, int enc1, int enc2, void(*enc_int)(), double k_p, double k_i, double k_d) {
  Motor m;
  m.en = en;
  m.ph = ph;
  m.enc1 = enc1;
  m.enc2 = enc2;
  m.pwm = 0;
  m.dir = OFF;
  m.encCount = 0;
  m._lastEncCount = 0;
  m.encCountTarget = 0;

  pinMode(m.en, OUTPUT);
  pinMode(m.ph, OUTPUT);
  pinMode(m.enc1, INPUT);
  pinMode(m.enc2, INPUT);
  attachInterrupt(digitalPinToInterrupt(m.enc1), enc_int, CHANGE);
  attachInterrupt(digitalPinToInterrupt(m.enc2), enc_int, CHANGE);

  m.anglePID = new motorPID(OUTPUT_MIN, OUTPUT_MAX, k_p, k_i, k_d);

  return m;
}

void driveMotor(Motor* m) {
   switch (m->dir) {
    case CCW:
      digitalWrite(m->ph, HIGH);
      analogWrite(m->en, m->pwm);
      break;
    case CW:
      digitalWrite(m->ph, LOW);
      analogWrite(m->en, m->pwm);
      break;
    case OFF:
      digitalWrite(m->en, LOW);
      break;
   }
}

void setAngleTarget(Motor *m, int targetAngle) {
  m->encCountTarget = (targetAngle) * COUNTS_PER_REV / DEG_PER_REV;

}

void calcPID(Motor *m) {
  static unsigned long lastMilli = 0;
  unsigned long dt = millis() - lastMilli;
  if (dt > 50) {
    long currentEncoderCount = m->encCount;
    m->pwm = m->anglePID->compute(m->encCountTarget, m->encCount);
    if (m->pwm > 0) m->dir = CCW;
    else if (m->pwm < 0) m->dir = CW;
    else m->dir = OFF;
    
    lastMilli = millis();
  }  
}
