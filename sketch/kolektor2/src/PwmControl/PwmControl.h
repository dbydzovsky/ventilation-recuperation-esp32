#ifndef PwmControl_h
#define PwmControl_h
#include "Arduino.h"

#define PWM_CONTROL_FREQUENCY 25000
#define PWM_CONTROL_RESOLUTION 8

class PwmControl {
  public:
    PwmControl(int channel, int pin);
    void setDutyCycle(byte dutyCycle);
  private:
    int _channel;
};

#endif