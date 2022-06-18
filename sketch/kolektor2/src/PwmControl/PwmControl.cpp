#include "Arduino.h"
#include "PwmControl.h"
#include "../Constants/Constants.h"

PwmControl::PwmControl(int channel, int pin, int initialDuty)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  this->_pin = pin;
  this->_channel = channel;
  this->_initialDuty = initialDuty;
}

void PwmControl::setHz(int hz) {
  this->_hz = hz;
}

void PwmControl::setup() {
  ledcSetup(this->_channel, this->_hz, PWM_CONTROL_RESOLUTION);
  ledcAttachPin(this->_pin, this->_channel);
  ledcWrite(this->_channel, this->_initialDuty);
}  

void PwmControl::setDutyCycle(byte duty) {
  if (this->_dutyCycle != duty) {
    this->_dutyCycle = duty;
    ledcWrite(this->_channel, (int) duty);
  }
}
