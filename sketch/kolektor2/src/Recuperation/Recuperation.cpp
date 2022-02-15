#include "Arduino.h"
#include "Recuperation.h"
#include "../PwmControl/PwmControl.h"
#include "../Constants/Constants.h"
#include "../Relay/Relay.h"
#include "../RPMChecker/RPMChecker.h"

#define DISABLED_FAN_DUTY 125

Recuperation::Recuperation(Relay * relay, PwmControl *control, RPMChecker * checker)
{
  this->_control = control;
  this->_relay = relay;
  this->_mode = RECUPERATION_MODE_RECYCLE;
  this->_power = 0;
  this->_checker = checker;
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
  if (this->_checker->shouldStop()) {
    return 0;
  }
  if (this->_directionIn) {
    return (short) this->_power;
  } else {
    return ((short)(this->_power)) * -1;
  }
}

short Recuperation::getActualPower() {
  if (this->_changingDirection) {
    return 0;
  }
  return this->getPower();
}

void Recuperation::setCycleDuration(int duration) {
  this->_cycleDuration = duration;
}

void Recuperation::setDurationChangeWait(int duration) {
  this->_durationChangeWait = duration;
}

void Recuperation::act() {
  if (this->_power <= 0 || this->_power > 100 || this->_checker->shouldStop()) {
    this->_control->setDutyCycle(125);
    this->_relay->disable();
    this->_changingDirection = false;
    return;
  }
  this->_relay->enable();
  if (millis() - this->_last_direction_change > (this->_cycleDuration + this->_durationChangeWait)) {
    bool previousDirection = this->_directionIn;
    if (this->_mode == RECUPERATION_MODE_EXHALE) {
      this->_directionIn = false;
    }
    if (this->_mode == RECUPERATION_MODE_INHALE) {
      this->_directionIn = true;
    }
    if (this->_mode == RECUPERATION_MODE_RECYCLE) {
      this->_directionIn = !this->_directionIn;
    }
    if (previousDirection != this->_directionIn) {
      this->_last_direction_change = millis(); 
    }
  }
  if (millis() - this->_last_direction_change < this->_durationChangeWait) {
    this->_changingDirection = true;
    this->_control->setDutyCycle(DISABLED_FAN_DUTY);
  } else {
    this->_changingDirection = false;
    if (_directionIn) {
      int duty = map(this->_power, 0, 100, 150, 254);
      this->_control->setDutyCycle(duty);
    } else {
      int duty = map(this->_power, 0, 100, 100, 0);
      this->_control->setDutyCycle(duty);
    }

  }
}