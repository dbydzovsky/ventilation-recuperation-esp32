/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef TimeProvider_h
#define TimeProvider_h
#include "Arduino.h"

class TimeProvider {
  public:
    TimeProvider();
    bool isTimeSet();
    void updateTime(unsigned long unixTimestamp, int offset);
  private:
    bool _initialized = false;
};

#endif