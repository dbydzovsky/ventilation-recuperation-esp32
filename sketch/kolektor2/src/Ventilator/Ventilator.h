#ifndef Ventilator_h
#define Ventilator_h
#include "Arduino.h"
#include "../PwmControl/PwmControl.h"
#include "../RPMChecker/RPMChecker.h"
#include "../Relay/Relay.h"
#include "../Debugger/Debugger.h"

class Ventilator {
  public:
    Ventilator(Relay * relay, PwmControl *control, RPMChecker *checker, Debugger * debugger);
    void setPower(byte power);
    short getPower();
    short getIntendedPower();
    void act();
  private:
    bool _overtempInitialized = false;
    unsigned long _overTempSince;
    int _maxTemperature;
    byte _power;
    Debugger * _debugger;
    Relay * _relay;
    PwmControl * _control;
    RPMChecker * _checker;
};

#endif