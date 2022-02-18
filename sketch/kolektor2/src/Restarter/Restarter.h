/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Restarter_h
#define Restarter_h
#include "Arduino.h"

class Restarter {
  public:
    void act();
    void restart();
  private:
    bool _shouldRestart = false;
};

#endif