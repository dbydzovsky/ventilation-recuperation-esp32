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
    Average(Source * source);
    void setActual(float newValue);
    unsigned short getErrors();
    byte getWarnings();
    float getAverage();
    float getValue();
    void takeReading(short newval);
    void doReading();
  private:
    Source *source;
    short values[7] = {0, 0, 0, 0, 0, 0, 0};
    short avg = 0;
    short actual = 0;
    boolean initialized = false;
    boolean error = false;
    unsigned short errorsInRow = 0;
    unsigned short index = 0;
    byte warnings = 0;
    byte initSuccessLoopCount = 0;
    short readSingle();
    bool isInValidRange(short value);
    void addWarnReading();
    void addErrorReading();
    void startInitializing ();
    void handleUnitialized(short newVal);
    void handleNormal(short newVal);
};



#endif