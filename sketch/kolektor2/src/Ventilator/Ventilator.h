#ifndef Ventilator_h
#define Ventilator_h
#include "Arduino.h"
#include "../PwmControl/PwmControl.h"
#include "../RPMChecker/RPMChecker.h"
#include "../Relay/Relay.h"

class Ventilator {
  public:
    Ventilator(Relay * relay, PwmControl *control, RPMChecker *checker);
    void setPower(byte power);
    short getPower();
    void act();
  private:
    byte _power;
    Relay * _relay;
    PwmControl * _control;
    RPMChecker * _checker;
};

#endif