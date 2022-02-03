/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Button_h
#define Button_h
#include "Arduino.h"


#ifndef BUTTON_COOLDOWN_MS
#define BUTTON_COOLDOWN_MS 1000
#endif

#ifndef BUTTON_HOLD_THRESHOLD
#define BUTTON_HOLD_THRESHOLD 3000
#endif

class ButtonHandler {
public:
    virtual boolean handleHold(int durationMillis, boolean finished) = 0;
    virtual int handleClick(byte clicks) = 0;
    virtual ~ButtonHandler() {};
};


class Button {
  public:
    Button(int pin, ButtonHandler *handler);
    boolean isPressed();
    void act();
  private:
    void resetState();
    ButtonHandler *_handler;
    int _pin;
    boolean _last_value = false;
    unsigned long _btn_pressed_time = 0;
    unsigned long _btn_unpressed_time = 0;
    byte _clicks = 0;
};

#endif