/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Display_h
#define Display_h
#include "Arduino.h"

#include <Adafruit_SSD1306.h>
#include "Arduino.h"
#include "../Dependencies/Dependencies.h"

class Display {
  public:
    Display(Dependencies * deps);
    void setup();
    void act();
  private:
    Adafruit_SSD1306 * d;
    Dependencies * deps;
    int index = 0;
};

#endif