#include "Arduino.h"
#include "Recuperation.h"
#include "../PwmControl/PwmControl.h"
#include "../Constants/Constants.h"

Recuperation::Recuperation(PwmControl *control)
{
  _control = control;
  _mode = RECUPERATION_MODE_RECYCLE;
  _power = 0;
}

void Recuperation::setPower(byte power, byte mode) {
  if (power != this->_power && mode != this->_mode) {
    if (IS_DEBUG) Serial.printf("Setting recuperation power to: %d, mode to: %d", this->_power, this->_mode);
    this->_mode = mode;
    this->_power = power;
  }
}

void Recuperation::act() {

}