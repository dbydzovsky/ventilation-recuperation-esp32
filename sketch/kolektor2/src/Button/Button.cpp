#include "Arduino.h"
#include "Button.h"
#include "../Display/Display.h"

Button::Button(int pin, Display *handler) {
  this->_pin = pin;
  this->_handler = handler;
  pinMode(pin, INPUT);
}

bool Button::isPressed() {
  return this->_last_value;
}

void Button::resetState() {
  this->_btn_pressed_time = NAN;
  this->_btn_unpressed_time = NAN;
  this->_clicks = 0;
}

void Button::act() {
  bool pressed = digitalRead(this->_pin);
  if (!pressed && this->_btn_pressed_time == 0) {
    return;
  }
  unsigned long aNow = millis();
  // press down edge
  if (pressed && !this->_last_value) {
    this->_handler->onPressDown();
    this->_btn_pressed_time = aNow;
  }

  // release edge
  int duration = (int)(aNow - this->_btn_pressed_time);
  if (pressed && this->_last_value && duration > ((int)BUTTON_HOLD_THRESHOLD)) {
    if (this->_handler->handleHold(duration, false)) {
      this->resetState();
      this->_last_value = true;
      return;
    }
  }
  if (!pressed && this->_last_value) {
    if (duration > ((int)BUTTON_HOLD_THRESHOLD)) {
      this->_handler->handleHold(duration, true);
      this->resetState();
      this->_last_value = pressed;
      return;
    } else {
      this->_clicks = this->_clicks + 1;
      this->_btn_unpressed_time = aNow;
    }
  }

  if (!pressed && !isnan(this->_btn_unpressed_time) && (aNow - this->_btn_unpressed_time > BUTTON_COOLDOWN_MS)) {
    this->_handler->handleClick((byte) this->_clicks);
    this->resetState();
  }
  this->_last_value = pressed;
}