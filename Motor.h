#ifndef MOTOR_H
#define MOTOR_H

#include "PID.h"

#define COUNTS_PER_REV 1200
#define DEG_PER_REV   360
#define OUTPUT_MAX    255
#define OUTPUT_MIN    -255

typedef enum {
  CW, CCW, OFF
} MOTOR_DIR;

typedef struct {
  int en, ph;
  int enc1, enc2;
  int pwm;
  MOTOR_DIR dir;
  int _lastEncCount;
  int encCount;  
  int encCountTarget;
  motorPID *anglePID;
} Motor;


Motor motor_init(int en, int ph, int enc1, int enc2, void(*enc_int)(), double k_p, double k_i, double k_d);
void driveMotor(Motor* m);
void setAngleTarget(Motor* m, int targetAngle);
void calcPID(Motor* m);

#endif
