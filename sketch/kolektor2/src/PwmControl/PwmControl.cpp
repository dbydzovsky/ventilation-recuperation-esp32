#include "Arduino.h"
#include "PwmControl.h"
PwmControl::PwmControl(int channel, int pin)
{
  pinMode(pin, OUTPUT);
  ledcSetup(channel, PWM_CONTROL_FREQUENCY, PWM_CONTROL_RESOLUTION);
  ledcAttachPin(pin, channel);
  this->_channel = channel;
}

void PwmControl::setDutyCycle(byte duty) {
  ledcWrite(this->_channel, (int) duty);
}
