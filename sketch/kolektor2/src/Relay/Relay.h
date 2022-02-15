#ifndef Relay_h
#define Relay_h
#include "Arduino.h"

class Relay {
  public:
    Relay(int pin);
    void act();
    void enable();
    void disable();
    void setCooldown(int cooldown);
  private:
    int _cooldown = 0;
    int _pin;
    bool _state = false;
    unsigned long _last_changed = millis();
    bool _should_be_enabled = false;
};

#endif