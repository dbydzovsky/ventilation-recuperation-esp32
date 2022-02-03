#ifndef Ventilator_h
#define Ventilator_h
#include "Arduino.h"
#include "../PwmControl/PwmControl.h"

class Ventilator {
  public:
    Ventilator(PwmControl *control);
    void setPower(byte power);
    void act();
  private:
    byte _power;
    PwmControl * _control;
};

#endif