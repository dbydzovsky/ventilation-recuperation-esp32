#ifndef Ventilator_h
#define Ventilator_h
#include "Arduino.h"
#include "../PwmControl/PwmControl.h"
#include "../RPMChecker/RPMChecker.h"

class Ventilator {
  public:
    Ventilator(PwmControl *control, RPMChecker *checker);
    void setPower(byte power);
    short getPower();
    void act();
  private:
    byte _power;
    PwmControl * _control;
    RPMChecker * _checker;
};

#endif