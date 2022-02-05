#include "Arduino.h"
#include "Sensors.h"
#include "../Constants/Constants.h"
class InsideSensor {
  public:
    InsideSensor() {

    }
    float readCo2() {
      return 400;
    }
    float readTemp() {
      return 20;
    }
};
class OutsideSensor {
  public:
    OutsideSensor() {

    }
    float readHum() {
      return 50;
    } 
    float readTemp() {
      return 20;
    }
};

class Co2Sensor: public Source {
  private:
    InsideSensor * _sensor;
  public:
    Co2Sensor(InsideSensor * sensor) {
      this->_sensor = sensor;
    }
    float getValue() {
      return 1000;
    };
    int getToleration() {
      return 200;
    };
    void setValue(float newVal) {
      
    }
};

class TempInsideSensor: public Source {
  private:
    InsideSensor * _sensor;
  public:
    TempInsideSensor(InsideSensor * sensor) {

    }
    float getValue() {
      return 24;
    };
    int getToleration() {
      return 5;
    };
    void setValue(float newVal) {
      
    }
};
class TempOutsideSensor: public Source {
  private:
    OutsideSensor * _sensor;
  public:
    TempOutsideSensor(OutsideSensor * sensor) {
      this->_sensor = sensor;
    }
    float getValue() {
      return 20;
    };
    int getToleration() {
      return 10;
    };
    void setValue(float newVal) {
      
    }
};

class HumOutsideSensor: public Source {
  private:
    OutsideSensor * _sensor;
  public:
    HumOutsideSensor(OutsideSensor * sensor) {
      this->_sensor = sensor;
    }
    float getValue() {
      return 5;
    };
    int getToleration() {
      return 15;
    };
    void setValue(float newVal) {

    }
};

class TestingSensor: public Source {
  private:
    float _value = 0;
  public:
    TestingSensor(float initial) {
      this->_value = initial;
    }
    float getValue() {
      return this->_value;
    };
    int getToleration() {
      return 3000;
    };
    void setValue(float newVal) {
      this->_value = newVal;
    } 
};

Sensors::Sensors() {
  if (IS_DEBUG) {
    Serial.println("Configuring testing sensors");
    this->co2Inside = new TestingSensor(1000);
    this->insideTemp = new TestingSensor(20);
    this->outsideHum = new TestingSensor(5);
    this->outsideTemp = new TestingSensor(20);
  } else {
    InsideSensor * inside = new InsideSensor();
    OutsideSensor * outside = new OutsideSensor();
    this->co2Inside = new Co2Sensor(inside);
    this->insideTemp = new TempInsideSensor(inside);
    this->outsideHum = new HumOutsideSensor(outside);
    this->outsideTemp = new TempOutsideSensor(outside);
  } 
}
