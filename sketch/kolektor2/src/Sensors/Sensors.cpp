#include "Arduino.h"
#include "Sensors.h"
#include <MHZ19_uart.h>
#include "../Constants/Constants.h"

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
    MHZ19_uart * _mhz19;
  public:
    
    Co2Sensor(MHZ19_uart * mhz19) {
      this->_mhz19 = mhz19;
    }
    
    float getValue() {
      int ppm = this->_mhz19->getCO2PPM();
      if (ppm < 400 || ppm > 5000) {
        return INVALID_VALUE;
      }
      return (float) ppm;
    };
    
    int getToleration() {
      return 300;
    };

    void setValue(float newVal) {
      
    }
};

class TempInsideSensor: public Source {
  private:
    MHZ19_uart * _mhz19;
  public:
    TempInsideSensor(MHZ19_uart * mhz19) {
      this->_mhz19 = mhz19;
    }
    float getValue() {
      int temp = this->_mhz19->getTemperature();
      if (temp == -1) {
        return INVALID_VALUE;
      }
      if (temp < -20 || temp > 60) {
        return INVALID_VALUE;
      }
      return temp;
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

Sensors::Sensors(MHZ19_uart * mhz19) {
  if (TESTING_CO2_SENSOR) {
    this->co2Inside = new TestingSensor(1000);
    this->insideTemp = new TestingSensor(20);
  } else {
    this->co2Inside = new Co2Sensor(mhz19);
    this->insideTemp = new TempInsideSensor(mhz19);
  }
  if (IS_DEBUG) {
    Serial.println("Configuring testing sensors");
    this->outsideHum = new TestingSensor(5);
    this->outsideTemp = new TestingSensor(20);
  } else {
    OutsideSensor * outside = new OutsideSensor();
    this->outsideHum = new HumOutsideSensor(outside);
    this->outsideTemp = new TempOutsideSensor(outside);
  } 
}
