#include "Arduino.h"
#include "Ventilator.h"
#include "../PwmControl/PwmControl.h"
#include "../Constants/Constants.h"
#include "../RPMChecker/RPMChecker.h"

Ventilator::Ventilator(PwmControl * control, RPMChecker * checker)
{
  this->_control = control;
  this->_checker = checker;
  this->_power = 0;
}

short Ventilator::getPower() {
  if (this->_checker->shouldStop()) {
    return 0;
  }
  return (short) this->_power;
}

void Ventilator::act() {
  short duty = map(this->_power, 0, 100, 0, 254);
  if (this->_checker->shouldStop()) {
    this->_control->setDutyCycle(0);
  }
  this->_control->setDutyCycle((byte) duty);
}

void Ventilator::setPower(byte power) {
  if (power != this->_power) {
    if (power < 10 || power > 100) {
      this->_power = 0;
    } else {
      this->_power = power;
    }
    if (IS_DEBUG) Serial.printf("Setting ventilator power to: %d", power);
    if (IS_DEBUG) Serial.println("");
  }
}