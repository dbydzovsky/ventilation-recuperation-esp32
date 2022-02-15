#include "Arduino.h"
#include "Relay.h"
#include "../Constants/Constants.h"
Relay::Relay(int pin)
{
  digitalWrite(pin, HIGH);
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
        if (IS_DEBUG) Serial.println("Turning relay on.");
        digitalWrite(this->_pin, LOW);
      } else {
        if (IS_DEBUG) Serial.println("Turning relay off.");
        digitalWrite(this->_pin, HIGH);
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