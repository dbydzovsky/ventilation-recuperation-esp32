#include "Arduino.h"
#include "Programme.h"
#include "../Debugger/Debugger.h"
#include "DisabledProgramme.cpp"
#include "SummerProgramme.cpp"
#include "WinterProgramme.cpp"
#include "ErrorProgramme.cpp"
#include "TrialProgramme.cpp"
#include "InitialProgramme.cpp"
#include "AutoProgramme.cpp"
#include "RecuperationProgramme.cpp"

ProgrammeFactory::ProgrammeFactory(Debugger * debugger) {
    this->Recuperation = new RecuperationProgramme(debugger);
    this->Disabled = new DisabledProgramme(debugger);
    this->Summer = new SummerProgramme(debugger, this->Recuperation);
    this->Winter = new WinterProgramme(debugger, this->Recuperation);
    this->Initial = new InitialWarmingUpProgramme(debugger);
    this->Error = new ErrorProgramme(debugger);
    this->Auto = new AutoProgramme(debugger, this);
    this->Trial = new TrialProgramme(debugger);
}