/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Orchestrator_h
#define Orchestrator_h
#include "Arduino.h"
#include "../Programme/Programme.h"
#include "../Lock/Lock.h"
#include "../Ventilator/Ventilator.h"
#include "../Recuperation/Recuperation.h"
#include "../RGBDiode/RGBDiode.h"

struct OrchestratorDependencies {
  Ventilator * ventilation;
  Recuperation * recuperation;
  Lock * lock;
  ProgrammeFactory * factory;
  RGBDiode * diode;
  ConfigurationData * data;
  Configuration * conf;
};

class Orchestrator  {
  public:
    Orchestrator(OrchestratorDependencies * deps);
    void getProgrammeName(char dest[80]);
    void setProgramme(Programme * programme);
    void assignProgramme();
    void act();
    bool handleHold(int durationMillis, bool finished);
    bool handleClick(byte times);
  private:
    OrchestratorDependencies * deps;
    int _programCode;
    Programme * actual;
};
    
#endif