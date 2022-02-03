#include "Arduino.h"
#include "RGBDiode.h"
#include <Ticker.h>

RGBDiode::RGBDiode(int a, int b, int c) {
  this->_a = a;
  this->_b = b;
  this->_c = c;
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
}
void RGBDiode::aOn() {
  if (this->_previous != 1) {
    digitalWrite(this->_b, LOW);
    digitalWrite(this->_c, LOW);
    digitalWrite(this->_a, HIGH);
    this->_previous = 1;
  }  
}
void RGBDiode::bOn() {
  if (this->_previous != 2) {
    digitalWrite(this->_c, LOW);
    digitalWrite(this->_a, LOW);
    digitalWrite(this->_b, HIGH);
    this->_previous = 2;
  } 
}
void RGBDiode::cOn() {
  if (this->_previous != 3) {
    digitalWrite(this->_a, LOW);
    digitalWrite(this->_b, LOW);
    digitalWrite(this->_c, HIGH);
    this->_previous = 3;
  } 
}
void RGBDiode::off() {
  if (this->_previous != 0) {
    digitalWrite(this->_a, LOW);
    digitalWrite(this->_b, LOW);
    digitalWrite(this->_c, LOW);
    this->_previous = 0;
  }  
}

void RGBDiode::tick() {
  this->_tickingIndex = (this->_tickingIndex + 1) % (this->_tickingSize);
  byte value = 0;
  if (this->_tickingIndex < (this->_tickingSize - this->_tickingConf[0])) {
    value = this->_tickingConf[this->_tickingIndex + 1];
  }
  if (value == 0) {
    this->off();
  } else if (value == 1) {
    this->aOn();
  } else if (value == 2) {
    this->bOn();
  } else {
    this->cOn();
  }
}

void RGBDiode::configure(byte tickingName, byte tickingSize, const byte ticking[8]) {
  if (this->_mode == RGBDIODE_PRIORITY_MODE) {
    return;
  }
  this->_mode = RGBDIODE_CLASSICAL_MODE;
  this->set(tickingName, tickingSize, ticking);
}
void RGBDiode::important(byte tickingName, byte tickingSize, const byte ticking[8]) {
  this->_mode = RGBDIODE_PRIORITY_MODE;
  this->set(tickingName, tickingSize, ticking);
}

void RGBDiode::set(byte tickingName, byte tickingSize, const byte ticking[8]) {
  if (tickingName != this->_tickingName) {
    this->detach();
    this->_tickingName = tickingName;
    for (byte i = 0; i < tickingSize; i++) {
      this->_tickingConf[i] = ticking[i];
    }
    this->_tickingIndex = 0;
    this->_tickingSize = (tickingSize - 1) + ticking[0];
    ticker.attach_ms(100, &RGBDiode::staticTickerCallbackLed, this);
  }
}
void RGBDiode::detach() {
  this->_mode = RGBDIODE_CLASSICAL_MODE;
}

void RGBDiode::staticTickerCallbackLed(RGBDiode *pThis)
{
    pThis->tick();
}