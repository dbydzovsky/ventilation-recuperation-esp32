/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Restarter_h
#define Restarter_h
#include "Arduino.h"
#include "../FilterMonitor/FilterMonitor.h"

class Restarter {
  public:
    Restarter(FilterMonitor * monitor);
    void act();
    void restart();
  private:
    FilterMonitor * _monitor;
    bool _shouldRestart = false;
};

#endif