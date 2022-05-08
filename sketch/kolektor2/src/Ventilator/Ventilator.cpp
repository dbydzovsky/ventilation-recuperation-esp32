#include "Arduino.h"
#include "Ventilator.h"
#include "../PwmControl/PwmControl.h"
#include "../Constants/Constants.h"
#include "../RPMChecker/RPMChecker.h"
#include "../Relay/Relay.h"
#include "../Debugger/Debugger.h"

Ventilator::Ventilator(Relay * relay, PwmControl * control, RPMChecker * checker, Debugger * debugger)
{
  this->_debugger = debugger;
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
short Ventilator::getIntendedPower() {
  return (short) this->_power;
}
void Ventilator::act() {
  if (this->_checker->shouldStop()) {
    this->_relay->disable();
    this->_control->setDutyCycle(0);
    return;
  }
  if (this->_power > 0 && this->_power <= 100) {
    this->_relay->enable();
    short duty = map(this->_power, 0, 100, 30, 254);
    this->_control->setDutyCycle((byte) duty);
  } else {
    this->_relay->disable();
    this->_control->setDutyCycle(0);
    return;
  }
}

void Ventilator::setPower(byte power) {
  if (power != this->_power) {
    if (power < 0 || power > 100) {
      this->_power = 0;
    } else {
      this->_power = power;
    }
  }
}