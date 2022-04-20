#include "Arduino.h"
#include "TimeProvider.h"
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time

#include "../Constants/Constants.h"
TimeProvider::TimeProvider(){}

bool TimeProvider::isTimeSet() {
  if (this->_initialized){
    return timeStatus() != timeNotSet;
  }
  return false;
}

void TimeProvider::updateTime(unsigned long unixTimestamp, int offset) {
  this->_initialized = true;
  if (IS_DEBUG) Serial.print("Updating time to: ");
  if (IS_DEBUG) Serial.println(unixTimestamp);
  setTime(unixTimestamp);
  adjustTime(offset);
}
