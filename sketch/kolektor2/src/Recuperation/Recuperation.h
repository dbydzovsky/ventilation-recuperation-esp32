/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Recuperation_h
#define Recuperation_h
#include "Arduino.h"
#include "../PwmControl/PwmControl.h"

class Recuperation {
  public:
    Recuperation(PwmControl *control);
    /**
     * @brief Set the Power object
     * 
     * @param power 0-100
     */
    void setPower(byte power, byte mode);

    void act();
  private:
    PwmControl * _control;
    byte _power;
    byte _mode;
};

#endif