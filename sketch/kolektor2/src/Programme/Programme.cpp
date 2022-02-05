#include "Arduino.h"
#include "Programme.h"
#include "DisabledProgramme.cpp"
#include "SummerProgramme.cpp"
#include "WinterProgramme.cpp"
#include "ErrorProgramme.cpp"
#include "TrialProgramme.cpp"
#include "InitialProgramme.cpp"
#include "AutoProgramme.cpp"
#include "RecuperationProgramme.cpp"

ProgrammeFactory::ProgrammeFactory() {
    this->Recuperation = new RecuperationProgramme();
    this->Disabled = new DisabledProgramme();
    this->Summer = new SummerProgramme(this->Recuperation);
    this->Winter = new WinterProgramme(this->Recuperation);
    this->Initial = new InitialWarmingUpProgramme();
    this->Error = new ErrorProgramme();
    this->Auto = new AutoProgramme(this);
    this->Trial = new TrialProgramme();
}