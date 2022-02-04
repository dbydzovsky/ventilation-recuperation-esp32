#include "Arduino.h"
#include "Sensors.h"

class Co2Sensor: public Source {
  public:
    Co2Sensor() {

    }
    float getValue() {
      return 400;
    };
    short getToleration() {
      return 200;
    };
};

class TempInsideSensor: public Source {
  public:
    TempInsideSensor() {

    }
    float getValue() {
      return 24;
    };
    short getToleration() {
      return 5;
    };
};
class TempOutsideSensor: public Source {
  public:
    TempOutsideSensor() {

    }
    float getValue() {
      return 60;
    };
    short getToleration() {
      return 10;
    };
};

class HumOutsideSensor: public Source {
  public:
    HumOutsideSensor() {

    }
    float getValue() {
      return 5;
    };
    short getToleration() {
      return 15;
    };
};


Sensors::Sensors() {
  this->co2Inside = new Co2Sensor();
  this->insideTemp = new TempInsideSensor();
  this->outsideHum = new HumOutsideSensor();
  this->outsideTemp = new TempOutsideSensor();
}
