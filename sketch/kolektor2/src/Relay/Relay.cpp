#include "Arduino.h"
#include "Relay.h"

Relay::Relay(int pin)
{
  digitalWrite(pin, HIGH);
  pinMode(pin, OUTPUT);
  this->_pin = pin;
}

void Relay::act() {

}

void Relay::enable() {
  digitalWrite(this->_pin, LOW);
}

void Relay::disable() {
  digitalWrite(this->_pin, HIGH);
}