#ifndef PID_h
#define PID_h

#include "arduino.h"

class motorPID{
  private:
    double _Kp, _Ki, _Kd;
    double  _integralError = 0; 
    double _prevMeasuredFeedback, _prevError;
    int _deadBandOffset = 0;
    int _samplingTime = 0; 
    int _outputMin, _outputMax;
    bool _isSpeedPID = true;
    unsigned long _lastMilli = 0;
    
  public:
    motorPID(int outputMin, int outputMax, double Kp, double Ki, double Kd );
    void setDeadBandOffset(int deadBandOffset, bool isSpeedPID);
    int compute(int _setPoint, int _measuredFeedback);
    void changeGains(double kp, double ki, double kd ); 
    void changeSamplingTime(int samplingTime);
};


#endif
