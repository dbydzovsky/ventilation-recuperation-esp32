#include "Arduino.h"
#include "PwmControl.h"
#include "../Constants/Constants.h"

PwmControl::PwmControl(int channel, int pin)
{
  pinMode(pin, OUTPUT);
  this->_pin = pin;
  this->_channel = channel;
}

void PwmControl::setHz(int hz) {
  this->_hz = hz;
}

void PwmControl::setup() {
  ledcSetup(this->_channel, this->_hz, PWM_CONTROL_RESOLUTION);
  ledcAttachPin(this->_pin, this->_channel);
}  

void PwmControl::setDutyCycle(byte duty) {
  if (this->_dutyCycle != duty) {
    this->_dutyCycle = duty;
    ledcWrite(this->_channel, (int) duty);
  }
}
