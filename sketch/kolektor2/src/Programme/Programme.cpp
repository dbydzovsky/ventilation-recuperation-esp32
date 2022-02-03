#include "Arduino.h"
#include "Programme.h"
#include "DisabledProgramme.cpp"
#include "SummerProgramme.cpp"
#include "WinterProgramme.cpp"
#include "ErrorProgramme.cpp"
#include "TrialProgramme.cpp"
#include "InitialProgramme.cpp"
#include "AutoProgramme.cpp"

ProgrammeFactory::ProgrammeFactory() {
    this->Disabled = new DisabledProgramme();
    this->Summer = new SummerProgramme();
    this->Winter = new WinterProgramme();
    this->Initial = new InitialWarmingUpProgramme();
    this->Error = new ErrorProgramme();
    this->Auto = new AutoProgramme(this);
    this->Trial = new TrialProgramme();
}