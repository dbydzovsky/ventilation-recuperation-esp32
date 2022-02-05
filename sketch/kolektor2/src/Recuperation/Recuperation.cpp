#include "Arduino.h"
#include "Recuperation.h"
#include "../PwmControl/PwmControl.h"
#include "../Constants/Constants.h"
#include "../Relay/Relay.h"

#define DISABLED_FAN_DUTY 125

Recuperation::Recuperation(Relay * relay, PwmControl *control)
{
  _control = control;
  _relay = relay;
  _mode = RECUPERATION_MODE_RECYCLE;
  _power = 0;
  this->_control->setDutyCycle(DISABLED_FAN_DUTY);
}

void Recuperation::setPower(byte power, byte mode) {
  if (power != this->_power || mode != this->_mode) {
    if (this->_power == 0) {
      // has been disabled
      this->_directionIn = mode == RECUPERATION_MODE_INHALE;
      this->_last_direction_change == millis();
    }
    if (IS_DEBUG) Serial.printf("Setting recuperation power to: %d, mode to: %d", power, this->_mode);
    if (IS_DEBUG) Serial.println("");
    this->_mode = mode;
    if (power <= 0 || power > 100) {
      this->_power = 0;
    } else {
      this->_power = power;
    }
  }
}

short Recuperation::getPower(){
  if (this->_directionIn) {
    return (short) this->_power;
  } else {
    return ((short)(this->_power)) * -1;
  }
}

void Recuperation::act() {
  if (this->_power == 0) {
    this->_control->setDutyCycle(125);
    this->_relay->disable();
    return;
  }
  this->_relay->enable();
  if (millis() - this->_last_direction_change > (RECUPERATION_CYCLE_DURATION + RECUPERATION_WAIT_FOR_DIRECTION_CHANGE)) {
    if (this->_mode == RECUPERATION_MODE_EXHALE) {
      this->_directionIn = true;
    }
    if (this->_mode == RECUPERATION_MODE_INHALE) {
      this->_directionIn = false;
    }
    if (this->_mode == RECUPERATION_MODE_RECYCLE) {
      this->_directionIn = !this->_directionIn;
    }
    this->_last_direction_change = millis(); 
  }
  if (millis() - this->_last_direction_change < RECUPERATION_WAIT_FOR_DIRECTION_CHANGE) {
    this->_control->setDutyCycle(DISABLED_FAN_DUTY);
  } else {
    int duty = DISABLED_FAN_DUTY;
    if (_directionIn) {
      duty = map(this->_power, 0, 100, 150, 254);
    } else {
      duty = map(this->_power, 0, 100, 100, 0);
    }
    this->_control->setDutyCycle(duty);
  }
}