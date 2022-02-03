#ifndef Relay_h
#define Relay_h
#include "Arduino.h"

class Relay {
  public:
    Relay(int pin);
    void act();
    void enable();
    void disable();
  private:
    int _pin;
};

#endif