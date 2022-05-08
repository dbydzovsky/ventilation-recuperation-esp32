#include "Arduino.h"
#include "Relay.h"
#include "../Constants/Constants.h"
#include "../Debugger/Debugger.h"
Relay::Relay(Debugger * debugger)
{
  this->debugger = debugger;
}

void Relay::setPin(int pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
  this->_pin = pin;
}

void Relay::setCooldown(int cooldown) {
  this->_cooldown = cooldown;
}

void Relay::act() {
  if (millis() - this->_last_changed > this->_cooldown) {
    this->_last_changed = millis();
    if (this->_state != this->_should_be_enabled) {
      this->_state = this->_should_be_enabled;
      if (this->_state) {
        this->debugger->trace("Turning relay on.");
        digitalWrite(this->_pin, HIGH);
      } else {
        this->debugger->trace("Turning relay off.");
        digitalWrite(this->_pin, LOW);
      }
    }
  }
}

void Relay::enable() {
  this->_should_be_enabled = true;
}

void Relay::disable() {
  this->_should_be_enabled = false;
}