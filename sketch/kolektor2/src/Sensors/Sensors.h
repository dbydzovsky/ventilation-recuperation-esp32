/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Morse_h
#define Morse_h
#include "Arduino.h"

class Source {
  public:
    virtual float getValue() = 0;
    virtual short getToleration() = 0;
};

class Sensors {
  public:
    Sensors();
    Source * outsideTemp;
    Source * outsideHum;
    Source * insideTemp;
    Source * co2Inside;
};

#endif