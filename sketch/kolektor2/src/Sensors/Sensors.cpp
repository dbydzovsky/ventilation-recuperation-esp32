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
};

class TestingSensor: public Testable {
  private:
    float _value = 0;
    int _randomInc = 0;
    int _min = 0;
    int _max = 0;
    Source * _decorated;
    bool _isTesting = false;
  public:
    TestingSensor(Source * decorated, float initial, int randomInc, int min, int max) {
      this->_value = initial;
      this->_randomInc = randomInc * 2;
      this->_decorated = decorated;
      this->_min = min;
      this->_max = max;
    }
    void setTesting() {
      this->_isTesting = true;
    }
    void setup() {
      if (!this->_isTesting) {
        this->_decorated->setup();
      }
    }
    void setValue(float val) {
      this->_value = val;
    }
    float getValue() {
      if (!this->_isTesting) {
        return this->_decorated->getValue();
      }
      bool change = (rand() % 100) > 70;
      if (change) {
        int changeVal = (rand() % this->_randomInc) - (this->_randomInc / 2);
        float newVal = this->_value + changeVal;
        if (newVal > this->_min && newVal < this->_max) {
          this->_value = newVal;
        } 
      }
      return this->_value;
    };
    int getToleration() {
      if (!this->_isTesting) {
        return 3000;
      }
      return this->_decorated->getToleration();
    };
};

Sensors::Sensors(MHZ19_uart * mhz19) {
  Adafruit_SHT31 * sht31 = new Adafruit_SHT31();
  uFire_SHT20 * sht20 = new uFire_SHT20();
  Source * insideTempSource = new TempInsideSensor(sht31);
  this->insideTemp = new TestingSensor(insideTempSource, 20, 4, 18, 26);
  Source * insideHumSource = new HumInsideSensor(sht31);
  this->insideHum = new TestingSensor(insideHumSource, 60, 6, 40, 60);
  Source * co2Source = new Co2Sensor(mhz19);
  this->co2Inside = new TestingSensor(co2Source, 1000, 60, 400, 5000);
  OutsideSensor * outside = new OutsideSensor(sht20);
  Source * outsideHumSource= new HumOutsideSensor(outside);
  this->outsideHum = new TestingSensor(outsideHumSource, 60, 6, 0, 100);
  Source * outsideTempSource = new TempOutsideSensor(outside);
  this->outsideTemp = new TestingSensor(outsideTempSource, 20, 2, -50, 130);
}

void Sensors::setTesting(){ 
  this->outsideTemp->setTesting();
  this->outsideHum->setTesting();
  this->insideTemp->setTesting();
  this->insideHum->setTesting();
  this->co2Inside->setTesting();
}

void Sensors::setup() {
  this->insideHum->setup();
  this->insideTemp->setup();
  this->outsideTemp->setup();
  this->outsideHum->setup();
  this->co2Inside->setup();
}