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
  setTime(unixTimestamp);
  adjustTime(offset);
}
