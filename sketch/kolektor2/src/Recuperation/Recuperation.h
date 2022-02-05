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
class Recuperation {
  public:
    Recuperation(Relay * relay, PwmControl *control);
    /**
     * @brief Set the Power object
     * 
     * @param power 0-100
     */
    void setPower(byte power, byte mode);

    void act();
    short getPower();
  private:
    unsigned long _last_direction_change = millis();
    PwmControl * _control;
    Relay * _relay;
    byte _power;
    byte _mode;
    bool _directionIn = false;
};

#endif