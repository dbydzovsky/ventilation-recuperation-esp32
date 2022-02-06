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
    void act();
  private:
    unsigned long last_retrival = millis();
    void doReport();
    Dependencies * _deps;
    Orchestrator * _orchestrator;
};

#endif