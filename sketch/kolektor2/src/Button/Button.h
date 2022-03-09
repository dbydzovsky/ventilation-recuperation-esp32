/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Button_h
#define Button_h
#include "Arduino.h"
#include "../Display/Display.h"

#ifndef BUTTON_COOLDOWN_MS
#define BUTTON_COOLDOWN_MS 300
#endif

#ifndef BUTTON_HOLD_THRESHOLD
#define BUTTON_HOLD_THRESHOLD 500
#endif

class Button {
  public:
    Button(int pin, Display *display);
    bool isPressed();
    void act();
  private:
    void resetState();
    Display *_handler;
    int _pin;
    bool _last_value = false;
    unsigned long _btn_pressed_time = 0;
    unsigned long _btn_unpressed_time = 0;
    byte _clicks = 0;
};

#endif