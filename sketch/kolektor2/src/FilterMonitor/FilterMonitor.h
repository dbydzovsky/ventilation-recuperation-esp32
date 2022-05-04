/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef FilterMonitor_h
#define FilterMonitor_h
#include "Arduino.h"
#include "../Recuperation/Recuperation.h"
#include "../Ventilator/Ventilator.h"
#include "../Constants/Constants.h"
#include "../Debugger/Debugger.h"
struct FilterReport {
  bool needCleaning = false;
  long remainMinutes = 0;
};

struct FilterData {
  long minutes = 0;
};

class FilterMonitor {
  public:
    FilterMonitor(Ventilator * ventilator, Recuperation *recuperation, Debugger * debugger);
    bool cleared(int filter);
    void report(byte filter, FilterReport * report);
    void act();
    void persist();
    void setup();
  private:
    Debugger * debugger;
    bool _ventilatorChanged = false;
    bool _recuperationChanged = false;
    long _ventilatorMinutes = 0;
    long _recuperationMinutes = 0;
    unsigned long _lastTracking = millis();
    unsigned long _lastPersistence = millis();
    Ventilator * _ventilator;
    Recuperation * _recuperation;
};

#endif