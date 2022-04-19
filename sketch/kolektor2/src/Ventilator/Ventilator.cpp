#include "Arduino.h"
#include "Ventilator.h"
#include "../PwmControl/PwmControl.h"
#include "../Constants/Constants.h"
#include "../RPMChecker/RPMChecker.h"
#include "../Relay/Relay.h"

Ventilator::Ventilator(Relay * relay, PwmControl * control, RPMChecker * checker)
{
  this->_relay = relay;
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
  short duty = 0;
  if (this->_power > 0) {
    duty = map(this->_power, 0, 100, 30, 254);
  }
  if (this->_checker->shouldStop()) {
    this->_relay->disable();
    this->_control->setDutyCycle(0);
  }
  if (this->_power != 0) {
    this->_relay->enable();
  }
  this->_control->setDutyCycle((byte) duty);
}

void Ventilator::setPower(byte power) {
  if (power != this->_power) {
    if (power < 0 || power > 100) {
      this->_power = 0;
    } else {
      this->_power = power;
    }
    if (IS_DEBUG) Serial.printf("Setting ventilator power to: %d", power);
    if (IS_DEBUG) Serial.println("");
  }
}