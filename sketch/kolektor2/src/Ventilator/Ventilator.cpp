#include "Arduino.h"
#include "Ventilator.h"
#include "../PwmControl/PwmControl.h"
#include "../Constants/Constants.h"

Ventilator::Ventilator(PwmControl * control)
{
  _control =  control;
  _power = 0;
}

void Ventilator::act() {
  // this->_control.setDutyCycle(50);
}

void Ventilator::setPower(byte power) {
  if (power != this->_power) {
    if (IS_DEBUG) Serial.printf("Setting ventilator power to: %d", this->_power);
    this->_power = power;
  }
}