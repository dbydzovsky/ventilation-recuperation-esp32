/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Recuperation_h
#define Recuperation_h
#include "Arduino.h"
#include "../PwmControl/PwmControl.h"
#include "../Relay/Relay.h"
#include "../RPMChecker/RPMChecker.h"

class Recuperation {
  public:
    Recuperation(Relay * relay, PwmControl *control, RPMChecker * checker);
    /**
     * @brief Set the Power object
     * 
     * @param power 0-100
     */
    void setPower(byte power, byte mode);

    void act();
    short getPower();
    short getActualPower();
    void setDurationChangeWait(int duration);
    void setCycleDuration(int duration);
  private:
    int _durationChangeWait = 6000;
    int _cycleDuration = 70000;
    unsigned long _last_direction_change = millis();
    PwmControl * _control;
    RPMChecker * _checker;
    Relay * _relay;
    byte _power;
    byte _mode;
    bool _directionIn = false;
    bool _changingDirection = false;
};

#endif