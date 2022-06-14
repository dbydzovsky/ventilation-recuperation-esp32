/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Sensors_h
#define Sensors_h
#include "Arduino.h"
#include <MHZ19_uart.h>

class Source {
  public:
    virtual void setup() = 0;
    virtual float getValue() = 0;
    virtual int getToleration() = 0;
};

class Testable: public Source {
  public:
    virtual void setTesting() = 0;
    virtual void setValue(float val) = 0;
};

class Sensors {
  public:
    Sensors(MHZ19_uart * mhz19);
    void setup();
    void setTesting();
    Testable * outsideTemp;
    Testable * outsideHum;
    Testable * insideTemp;
    Testable * insideHum;
    Testable * co2Inside;
};

#endif