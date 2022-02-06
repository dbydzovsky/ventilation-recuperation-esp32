/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Orchestrator_h
#define Orchestrator_h
#include "Arduino.h"
#include "../Programme/Programme.h"
#include "../Dependencies/Dependencies.h"

class Orchestrator  {
  public:
    Orchestrator(Dependencies * deps);
    void getProgrammeName(char dest[80]);
    void setProgramme(Programme * programme);
    void assignProgramme();
    void act();
    bool handleHold(int durationMillis, bool finished);
    bool handleClick(byte times);
    int getProgrammeCode();
  private:
    Dependencies * deps;
    int _programCode;
    Programme * actual;
    int validateCommonSense();
};
    
#endif