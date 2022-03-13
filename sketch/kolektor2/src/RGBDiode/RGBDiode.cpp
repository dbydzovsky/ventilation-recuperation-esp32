#include "Arduino.h"
#include "RGBDiode.h"
#include <Ticker.h>

RGBDiode::RGBDiode(int channel, int a, int b, int c) {
  this->_a = a;
  this->_b = b;
  this->_c = c;
  this->_channel = channel;
  // pinMode(a, OUTPUT);
  // pinMode(b, OUTPUT);
  // pinMode(c, OUTPUT);
  // ledcSetup(channel, 25000, 8);
  // ledcWrite(this->_channel, 255);
}

void RGBDiode::setBrightness(int brightness) {
  // this->_brightness = brightness;
  // int duty = map(this->_brightness, 0, 100, 0, 254);
  // ledcWrite(this->_channel, duty);
}

void RGBDiode::aOn() {
  if (this->_previous != 1) {
    // ledcDetachPin(this->_b);
    // ledcDetachPin(this->_c);
    // ledcAttachPin(this->_a, this->_channel);
    this->_previous = 1;
  }  
}
void RGBDiode::bOn() {
  if (this->_previous != 2) {

    // ledcDetachPin(this->_c);
    // ledcDetachPin(this->_a);
    // ledcAttachPin(this->_b, this->_channel);
    this->_previous = 2;
  } 
}
void RGBDiode::cOn() {
  if (this->_previous != 3) {
    // ledcDetachPin(this->_a);
    // ledcDetachPin(this->_b);
    // ledcAttachPin(this->_c, this->_channel);
    this->_previous = 3;
  } 
}
void RGBDiode::off() {
  if (this->_previous != 0) {
    // ledcDetachPin(this->_a);
    // ledcDetachPin(this->_b);
    // ledcDetachPin(this->_c);
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

void RGBDiode::configure(void (*configuration)(TickingConfiguration *out)) {
  if (this->_mode == RGBDIODE_PRIORITY_MODE) {
    return;
  }
  this->_mode = RGBDIODE_CLASSICAL_MODE;
  this->set(configuration);
}

void RGBDiode::important(void (*configuration)(TickingConfiguration *out)) {
  this->_mode = RGBDIODE_PRIORITY_MODE;
  this->set(configuration);
}

void RGBDiode::set(void (*configuration)(TickingConfiguration *out)) {
  if (configuration != this->_configuration) {
    this->detach();
    this->_configuration = configuration;
    TickingConfiguration conf;
    configuration(&conf);
    for (byte i = 0; i < conf.size; i++) {
      this->_tickingConf[i] = conf.ticking[i];
    }
    this->_tickingIndex = 0;
    this->_tickingSize = (conf.size - 1) + conf.ticking[0];
    // ticker.attach_ms(100, &RGBDiode::staticTickerCallbackLed, this);
  }
}
void RGBDiode::detach() {
  this->_mode = RGBDIODE_CLASSICAL_MODE;
}

void RGBDiode::staticTickerCallbackLed(RGBDiode *pThis)
{
    pThis->tick();
}

void tickingWiFiConnecting(TickingConfiguration *out) {
  out->ticking[0] = 1;
  out->ticking[1] = DIODE_GREEN_COLOR;
  out->ticking[2] = DIODE_RED_COLOR;
  out->ticking[3] = DIODE_BLUE_COLOR;
  out->size = 4;
}
void tickingWarmingUp(TickingConfiguration *out) {
  out->ticking[0] = 2;
  out->ticking[1] = DIODE_BLUE_COLOR;
  out->ticking[2] = DIODE_BLUE_COLOR;
  out->ticking[3] = DIODE_NONE_COLOR;
  out->ticking[4] = DIODE_BLUE_COLOR;
  out->size = 5;
}
void tickingTrial(TickingConfiguration *out) {
  out->ticking[0] = 2;
  out->ticking[1] = DIODE_GREEN_COLOR;
  out->ticking[2] = DIODE_NONE_COLOR;
  out->ticking[3] = DIODE_NONE_COLOR;
  out->ticking[4] = DIODE_BLUE_COLOR;
  out->size = 5;
}
void tickingDeviceEnabledWifiOn(TickingConfiguration *out) {
  out->ticking[0] = 35;
  out->ticking[1] = DIODE_NONE_COLOR;
  out->ticking[2] = DIODE_GREEN_COLOR;
  out->ticking[3] = DIODE_NONE_COLOR;
  out->ticking[4] = DIODE_GREEN_COLOR;
  out->size = 5;
}
void tickingDeviceEnabledWifiOff(TickingConfiguration *out) {
  out->ticking[0] = 37;
  out->ticking[1] = DIODE_NONE_COLOR;
  out->ticking[2] = DIODE_GREEN_COLOR;
  out->size = 3;
}
void tickingError(TickingConfiguration *out) {
  out->ticking[0] = 1;
  out->ticking[1] = DIODE_RED_COLOR;
  out->size = 2;
}
void tickingEnablementRed(TickingConfiguration *out) {
  out->ticking[0] = 1;
  out->ticking[1] = DIODE_RED_COLOR;
  out->size = 2;
}
void tickingCritical(TickingConfiguration *out) {
  out->ticking[0] = 8;
  out->ticking[1] = DIODE_RED_COLOR;
  out->ticking[2] = DIODE_RED_COLOR;
  out->ticking[3] = DIODE_RED_COLOR;
  out->ticking[4] = DIODE_RED_COLOR;
  out->size = 5;
}
void tickingConditionsMet(TickingConfiguration *out) {
  out->ticking[0] = 25;
  out->ticking[1] = DIODE_GREEN_COLOR;
  out->ticking[2] = DIODE_GREEN_COLOR;
  out->ticking[3] = DIODE_BLUE_COLOR;
  out->ticking[4] = DIODE_BLUE_COLOR;
  out->size = 5;
}
void tickingSummerDeviceEnabledWifiOn(TickingConfiguration *out) {
  out->ticking[0] = 35;
  out->ticking[1] = DIODE_NONE_COLOR;
  out->ticking[2] = DIODE_BLUE_COLOR;
  out->ticking[3] = DIODE_NONE_COLOR;
  out->ticking[4] = DIODE_BLUE_COLOR;
  out->size = 5;
}
void tickingSummerDeviceEnabledWifiOff(TickingConfiguration *out) {
  out->ticking[0] = 37;
  out->ticking[1] = DIODE_NONE_COLOR;
  out->ticking[2] = DIODE_BLUE_COLOR;
  out->size = 3;
}
void tickingAutoProgrammeOn(TickingConfiguration *out) {
  out->ticking[0] = 35;
  out->ticking[1] = DIODE_GREEN_COLOR;
  out->ticking[2] = DIODE_RED_COLOR;
  out->ticking[3] = DIODE_BLUE_COLOR;
  out->size = 4;
}
void tickingDeviceDisabledWifiOn(TickingConfiguration *out) {
  out->ticking[0] = 35;
  out->ticking[1] = DIODE_RED_COLOR;
  out->ticking[2] = DIODE_NONE_COLOR;
  out->ticking[3] = DIODE_RED_COLOR;
  out->size = 4; 
}
void tickingDeviceDisabledWifiOff(TickingConfiguration *out) {
  out->ticking[0] = 35;
  out->ticking[1] = DIODE_NONE_COLOR;
  out->ticking[2] = DIODE_NONE_COLOR;
  out->ticking[3] = DIODE_RED_COLOR;
    out->size = 4;
}
void tickingRecuperation(TickingConfiguration *out) {
  // todo
  out->ticking[0] = 35;
  out->ticking[1] = DIODE_BLUE_COLOR;
  out->ticking[2] = DIODE_BLUE_COLOR;
  out->ticking[3] = DIODE_BLUE_COLOR;
  out->size = 4;
}