/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Weather_h
#define Weather_h
#include "Arduino.h"

class WeatherForecast {
  public:
    WeatherForecast();
    bool hasValidForecast();
    void act();
    bool shouldCoolInsides();
};

#endif