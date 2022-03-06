#include "Arduino.h"
#include "Average.h"
#include "../Sensors/Sensors.h"
#include "../Constants/Constants.h"


float average (int * values, float len) {
  int sum = 0;
  for (int i = 0 ; i < len ; i++)
    sum += values[i] ;
  return sum / len;
}

Average::Average(Source *source) {
  this->source = source;
}

int Average::readSingle() {
  float newVal = this->source->getValue();
  Serial.println(newVal);
  if (newVal == INVALID_VALUE) {
    this->error = true;
    return NAN;
  }
  return (int) (newVal * 100);
}

bool Average::isInValidRange(int value) {
  int toleration = this->source->getToleration() * 100;
  if (value > (this->avg + toleration) || value < (this->avg - toleration)) {
    return false;
  }
  return true;
}

void Average::addWarnReading() {
  this->errorsInRow++;
  this->warnings++;
}
void Average::addErrorReading() {
  this->errorsInRow++;
}
void Average::startInitializing () {
  this->initialized = false;
  this->initSuccessLoopCount = 0;
  this->index = 1;
}

void Average::handleUnitialized(int newVal) {
  if (this->initSuccessLoopCount == 0) {
    this->takeReading(newVal);
  } else {
    if (this->isInValidRange(newVal)) {
      this->takeReading(newVal);
    } else {
      this->addWarnReading();
    }
  }
  if (this->index == 0) {
    this->initSuccessLoopCount++;
  }
  if (this->initSuccessLoopCount >= 2) {
    this->initialized = true;
  }
}

void Average::handleNormal(int newVal) {
  if (this->isInValidRange(newVal)) {
    this->takeReading(newVal);
  } else {
    this->addWarnReading();
  }
  if (errorsInRow > 5) {
    this->startInitializing();
  }
}
void Average::setActual(float newValue) {
  for (int i = 0; i < averageValuesCount; i++) {
    this->values[i] = (int) (newValue * 100);
  }
  this->actual = (int) (newValue * 100);
  this->avg = (int) average(this->values, averageValuesCount);
}

unsigned int Average::getErrors() {
  return this->errorsInRow;
}

byte Average::getWarnings() {
  return this->warnings;
}

bool Average::isInitialized() {
  return this->initialized;
}

float Average::getAverage() {
  if (this->initialized) {
    return ((float)this->avg) / 100;
  }
  return NAN;
}

float Average::getValue() {
  if (this->initialized) {
    return ((float)this->actual) / 100;
  }
  return NAN;
}
void Average::takeReading(int newval) {
  this->errorsInRow = 0;
  this->values[this->index] = newval;
  this->actual = newval;
  this->avg = (int) average(this->values, averageValuesCount);
  this->index = (this->index + 1) % averageValuesCount;
}

void Average::doReading() {
  this->error = false;
  int newVal = this->readSingle();
  if (!this->error) {
    if (this->initialized) {
      this->handleNormal(newVal);
    } else {
      this->handleUnitialized(newVal);
    }
  } else {
    addErrorReading();
    if (this->initialized && this->errorsInRow > 5) {
      this->startInitializing();
    }
  }
  if (this->errorsInRow != 0) {
    float remaining = errorsInRow % 40;
    if (remaining == 20) {
      // todo resetI2C();
    }
    if (remaining == 0) {
      // todo resetSensorPower->resetIt();
    }
    if (errorsInRow % 64000 == 0) {
      // todo restart();
    }
  }
}
