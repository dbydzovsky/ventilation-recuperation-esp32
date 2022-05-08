#ifndef PwmControl_h
#define PwmControl_h
#include "Arduino.h"


#define PWM_CONTROL_RESOLUTION 8

class PwmControl {
  public:
    PwmControl(int channel, int pin);
    void setHz(int hz);
    void setup();
    void setDutyCycle(byte dutyCycle);
  private:
    int _channel;
    int _pin;
    int _hz = 25000;
    byte _dutyCycle = 0;
};

#endif