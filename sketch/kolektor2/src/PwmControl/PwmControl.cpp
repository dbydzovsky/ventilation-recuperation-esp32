#include "Arduino.h"
#include "PwmControl.h"
#include "../Constants/Constants.h"

PwmControl::PwmControl(int channel, int pin)
{
  pinMode(pin, OUTPUT);
  ledcSetup(channel, PWM_CONTROL_FREQUENCY, PWM_CONTROL_RESOLUTION);
  ledcAttachPin(pin, channel);
  this->_channel = channel;
}

void PwmControl::setDutyCycle(byte duty) {
  if (this->_dutyCycle != duty) {
    if (IS_DEBUG) Serial.print("Setting duty cycle: ");
    if (IS_DEBUG) Serial.println(duty);
    this->_dutyCycle = duty;
    ledcWrite(this->_channel, (int) duty);
  }
}
