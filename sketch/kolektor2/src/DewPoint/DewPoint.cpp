#include "Arduino.h"
#include "DewPoint.h"

DewPoint::DewPoint(){}

void DewPoint::compute(float humidity, float temperature) {
  if (!isnan(humidity) && !isnan(temperature)) {  
    float e = 2.71828;
    float powResult = pow(e, (17.67 * temperature) / (243.5 + temperature));
    float logResult = log((humidity / 100) * powResult);
    float dewPoint = (243.5 * logResult) / (17.67 - logResult);
    this->_dewPoint = (short) (dewPoint * 100);
  } else {
    this->_dewPoint = -20000;
  }
}

float DewPoint::getDewPoint() {
  if (this->_dewPoint == -20000) {
    return NAN;
  }
  return ((float) this->_dewPoint) / 100;
}
