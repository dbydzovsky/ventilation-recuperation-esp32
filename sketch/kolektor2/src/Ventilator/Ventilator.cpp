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

void Ventilator::setMaxTemperature(int maxTemperature) {
  this->_maxTemperature = maxTemperature;
}

short Ventilator::getPower() {
  if (this->_checker->shouldStop()) {
    return 0;
  }
  return (short) this->_power;
}

bool Ventilator::overHeated() {
  if (this->_overtempInitialized) {
    return millis() - this->_overTempSince > MAX_OVERTEMPERATURE_PERIOD;
  }
  return false;
}

void Ventilator::act(int actualTemp) {
  short duty = 0;
  bool overTempAlarm = false;
  if (actualTemp > this->_maxTemperature && this->_power > 0) {
    if (this->_overtempInitialized) {
      overTempAlarm = millis() - this->_overTempSince > MAX_OVERTEMPERATURE_PERIOD;
    } else {
      char messageBuf[100];
      sprintf(messageBuf, "WARN Ventilator motor stopped due to critical TEMPERATURE %d C", actualTemp);
      this->_debugger->debug(messageBuf);
      this->_overTempSince = millis();
      this->_overtempInitialized = true;
    }
  } else {
    _overtempInitialized = false;
  }
  if (this->_power > 0) {
    duty = map(this->_power, 0, 100, 30, 254);
  }
  if (this->_checker->shouldStop() || overTempAlarm) {
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