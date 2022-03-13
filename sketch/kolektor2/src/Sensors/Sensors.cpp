#include "Arduino.h"
#include "Sensors.h"
#include <MHZ19_uart.h>
#include "../Constants/Constants.h"
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "uFire_SHT20.h"

class OutsideSensor {
  private:
    uFire_SHT20 * _sht20;
  public:
    OutsideSensor(uFire_SHT20 * sht20) {
      this->_sht20 = sht20;
    }
    void setup(){

    }
    float readHum() {
      float result = this->_sht20->humidity();
      if (result > 100 || result < 0) {
        return INVALID_VALUE;
      }
      return result;
    } 
    float readTemp() {
      float temperature = this->_sht20->temperature();
      if (temperature > 125 || temperature < -80) {
        return INVALID_VALUE;
      }
      return temperature;
    }
};


// zelená V+
// modrá V-
// pwm červená
// txd černá
// rxd žlutá
class Co2Sensor: public Source {
  private:
    MHZ19_uart * _mhz19;
  public:
    Co2Sensor(MHZ19_uart * mhz19) {
      this->_mhz19 = mhz19;
    }
    void setup(){

    }
    float getValue() {
      int ppm = this->_mhz19->getCO2PPM();
      if (ppm < 400 || ppm > 5000) {
        return INVALID_VALUE;
      }
      return (float) ppm;
    };
    
    int getToleration() {
      return 1000;
    };

    void setValue(float newVal) {
      
    }
};

class TempInsideSensor: public Source {
  private:
    Adafruit_SHT31 * sht31;
  public:
    TempInsideSensor(Adafruit_SHT31 * sht31) {
      this->sht31 = sht31;
    }
    void setup() {
      sht31->begin(0x45);
      sht31->heater(false);
    }
    float getValue() {
      float t = this->sht31->readTemperature();
      if (isnan(t)) {
        return INVALID_VALUE;
      }
      if (t < -20 || t > 60) {
        return INVALID_VALUE;
      }
      return t;
    };
    int getToleration() {
      return 5;
    };
    void setValue(float newVal) {
      
    }
};

class HumInsideSensor: public Source {
  private:
    Adafruit_SHT31 * sht31;
  public:
    HumInsideSensor(Adafruit_SHT31 * sht31) {
      this->sht31 = sht31;
    }
    void setup(){

    }
    float getValue() {
      float h = this->sht31->readHumidity();
      if (isnan(h)) {
        return INVALID_VALUE;
      }
      if (h < 0 || h > 100) {
        return INVALID_VALUE;
      }
      return h;
    };
    int getToleration() {
      return 30;
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
    void setup() {

    }
    float getValue() {
      delay(10);
      return this->_sensor->readTemp();
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
    void setup() {

    }
    float getValue() {
      delay(10);
      return this->_sensor->readHum();
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
    void setup() {
      
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
  Adafruit_SHT31 * sht31 = new Adafruit_SHT31();
  uFire_SHT20 * sht20 = new uFire_SHT20();

  this->insideTemp = new TempInsideSensor(sht31);
  // this->insideTemp = new TestingSensor(20);
  this->insideHum = new HumInsideSensor(sht31);
  // this->insideHum = new TestingSensor(60);
  // this->co2Inside = new TestingSensor(1000);
  // this->insideTemp = new TestingSensor(20);
  this->co2Inside = new Co2Sensor(mhz19);
    // Serial.println("Configuring testing sensors");
  this->outsideHum = new TestingSensor(5);
  this->outsideTemp = new TestingSensor(20);
  this->outsideTemp = this->insideTemp;
  // OutsideSensor * outside = new OutsideSensor(sht20);
  // this->outsideHum = new HumOutsideSensor(outside);
  // this->outsideTemp = new TempOutsideSensor(outside);
}

void Sensors::setup() {
  this->insideHum->setup();
  this->insideTemp->setup();
  this->outsideTemp->setup();
  this->outsideHum->setup();
  this->co2Inside->setup();
}