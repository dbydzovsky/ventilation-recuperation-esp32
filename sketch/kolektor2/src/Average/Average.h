/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Average_h
#define Average_h
#include "Arduino.h"
#include "../Sensors/Sensors.h"

class Average {
  public:
    Average(Testable * source);
    void setActual(float newValue);
    unsigned int getErrors();
    byte getWarnings();
    float getAverage();
    float getValue();
    void takeReading(int newval);
    void doReading();
    bool isInitialized();
  private:
    Testable *source;
    int values[7] = {0, 0, 0, 0, 0, 0, 0};
    int avg = 0;
    int actual = 0;
    boolean initialized = false;
    boolean error = false;
    unsigned int errorsInRow = 0;
    unsigned int index = 0;
    byte warnings = 0;
    byte initSuccessLoopCount = 0;
    int readSingle();
    bool isInValidRange(int value);
    void addWarnReading();
    void addErrorReading();
    void startInitializing ();
    void handleUnitialized(int newVal);
    void handleNormal(int newVal);
};



#endif