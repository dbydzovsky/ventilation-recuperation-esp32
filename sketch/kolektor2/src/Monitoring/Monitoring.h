/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Monitoring_h
#define Monitoring_h
#include "Arduino.h"
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"
class Monitoring {
  public:
    Monitoring(Orchestrator* orchestrator, Dependencies * deps);
    void hideInternalTempHum();
    void hideCo2();
    void act();
  private:
    unsigned long last_retrival = millis();
    bool _hideCo2 = false;
    bool _hideInternalTempHum = false;
    void doReport();
    Dependencies * _deps;
    Orchestrator * _orchestrator;
};

#endif