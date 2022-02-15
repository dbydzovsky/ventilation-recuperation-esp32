#include "Arduino.h"
#include "TimeProvider.h"
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time

#include "../Constants/Constants.h"
TimeProvider::TimeProvider()
{
}

bool TimeProvider::isTimeSet() {
  return timeStatus() != timeNotSet;
}

void TimeProvider::updateTime(unsigned long unixTimestamp, int offset) {
  if (IS_DEBUG) Serial.print("Updating time to: ");
  if (IS_DEBUG) Serial.println(unixTimestamp);
  setTime(unixTimestamp);
  adjustTime(offset);
}
