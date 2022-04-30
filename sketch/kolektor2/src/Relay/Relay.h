#ifndef Relay_h
#define Relay_h
#include "Arduino.h"
#include "../Debugger/Debugger.h"

class Relay {
  public:
    Relay(Debugger * debugger, int pin);
    void act();
    void enable();
    void disable();
    void setCooldown(int cooldown);
  private:
    Debugger * debugger;
    int _cooldown = 3000; // 5 seconds
    int _pin;
    bool _state = false;
    unsigned long _last_changed = millis();
    bool _should_be_enabled = false;
};

#endif