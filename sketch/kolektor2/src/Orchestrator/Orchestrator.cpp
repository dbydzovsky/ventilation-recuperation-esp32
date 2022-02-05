#include "Arduino.h"
#include "Orchestrator.h"
#include "../Configuration/Configuration.h"
#include "../Constants/Constants.h"
#include "../Button/Button.h"
#include "../Dependencies/Dependencies.h"
#include "../Weather/Weather.h"

int translateCode(byte code, char dest[50]) {
  if (code == 51) {
    strcpy(dest, "Nevalidní hodnota z venkovního čidla teploty");
    return 45;
  } else if (code == 52) {
    strcpy(dest, "Nevalidní hodnota z vnitřního čidla teploty");
    return 44;
  } else if (code == 53) {
    strcpy(dest, "Nevalidní hodnota z venkovního čidla vlhkosti");
    return 46;
  }
};

Orchestrator::Orchestrator(Dependencies * deps) {
  this->deps = deps;
  if (IS_DEBUG) {
    this->actual = deps->factory->Disabled;  
  } else {
    this->actual = deps->factory->Initial;  
  }
}

void Orchestrator::getProgrammeName(char dest[80]) {
  if (this->_programCode < 60) {
    translateCode(this->_programCode, dest);
  } else {
    this->actual->getDescription(dest);
  }
}
void Orchestrator::setProgramme(Programme * programme) {
  programme->onStart();
  this->actual = programme;
}
void Orchestrator::assignProgramme() {
  if (IS_DEBUG) Serial.println("Assinging Programme");
  Programme * candidate = this->deps->factory->Disabled;
  if (IS_DEBUG) Serial.println("Disabled assigned for any case");
  if (this->deps->confLock->readLock()) {
    if (IS_DEBUG) Serial.println("Conf locked");
    if (!this->deps->conf->dataSet) {
      if (IS_DEBUG) Serial.println("Data not set");
      candidate = this->deps->factory->Error;
    } else if (this->deps->conf->getData()->mode == WINTER_MODE) {
      if (IS_DEBUG) Serial.println("Assigning winter");
      candidate = this->deps->factory->Winter;
    } else if (this->deps->conf->getData()->mode == SUMMER_MODE) {
      if (IS_DEBUG) Serial.println("Assinging Summer");
      candidate = this->deps->factory->Summer;
    } else if (this->deps->conf->getData()->mode == AUTO_MODE) {
      if (IS_DEBUG) Serial.println("Assinging Auto");  
      candidate = this->deps->factory->Auto;
    } else {
      if (IS_DEBUG) Serial.println("Assinging Disabled");  
      candidate = this->deps->factory->Disabled;
    }
    if (IS_DEBUG) Serial.println("Unlocking");
    this->deps->confLock->readUnlock();
  }
  if (this->actual != candidate) {
    if (IS_DEBUG) Serial.println("Actual programme has changed");
    this->actual = candidate;
    this->actual->onStart();
  }
}

bool Orchestrator::handleClick(byte times) {
  if (times == 2) {
    ConfigurableProgramme * trialProgramme = this->deps->factory->Trial;
    PowerOutput output;
    trialProgramme->setPower(output, (int) temporaryDisabledProgrammeDuration);
    this->setProgramme(dynamic_cast<Programme*>(trialProgramme));
    return true;
  }
  if (times == 3) {
    ConfigurableProgramme * trialProgramme = this->deps->factory->Trial;
    PowerOutput output;
    output.mode = POWER_OUTPUT_MODE_BOTH;
    output.ventilatorPower = 30;    
    output.recuperationPower = 100;
    output.recuperationMode = RECUPERATION_MODE_RECYCLE;
    trialProgramme->setPower(output, manual100ProgrammeDuration);
    this->setProgramme(dynamic_cast<Programme*>(trialProgramme));
    return true;
  }
  return this->actual->handleClick((byte)times);
}

bool Orchestrator::handleHold(int duration_ms, bool finished) {
  if (this->actual->handleHold(duration_ms, finished)) {
    return true;
  }
  if (duration_ms > 10000) {
    // todo restart();
  }
  if (finished) {
    this->deps->diode->detach();
    if (duration_ms > enablementTresholdMs) {
      if (this->deps->conf->dataSet) {
        this->deps->conf->setInactiveMode();
        this->assignProgramme();
      }
    }
  } else {
    if (duration_ms > enablementTresholdMs) {
      this->deps->diode->configure(tickingEnablementRed);
    }
  }
  return finished;
}

void Orchestrator::act() {
  if (!this->deps->confLock->readLock()) {
    return;
  }
  ConfigurationData * data = this->deps->conf->getData();
  if (!this->actual->isValid(data)) {
    this->assignProgramme();
  }
  ProgrammeContext context;
  context.dewPoint = 5;
  context.humidityOutside = 60;
  context.tempInside = 20;
  context.tempOutside = 60;
  context.isTimeSet = this->deps->timeProvider->isTimeSet();
  context.data = data;
  context.forecast = this->deps->forecast;
  WeatherDeps weatherDeps = {data, this->deps->timeProvider, this->deps->httpClient, this->deps->httpLock};
  context.weatherDeps = &weatherDeps;
  PowerOutput out;
  int programCode;
  if (this->actual->canForce()) {
    this->actual->configureTicking(this->deps->diode);
    this->actual->getPower(&context, &out);
    programCode = this->actual->getCode();
  } else {
    // todo programCode = validateCommonSense(data);
    if (programCode == 0) {
      this->actual->configureTicking(this->deps->diode);
      if (IS_DEBUG) Serial.println("Getting power");
      this->actual->getPower(&context, &out);
      if (IS_DEBUG) Serial.println("Getting code");
      programCode = this->actual->getCode();
    } else {
      this->deps->diode->configure(tickingError);
    }
  }
  this->deps->confLock->readUnlock();
  if (out.mode == POWER_OUTPUT_MODE_VENTILATION || out.mode == POWER_OUTPUT_MODE_BOTH) {
    this->deps->ventilation->setPower(out.ventilatorPower);
  }
  if (out.mode == POWER_OUTPUT_MODE_RECUPERATION || out.mode == POWER_OUTPUT_MODE_BOTH) {
    this->deps->recuperation->setPower(out.recuperationPower, out.recuperationMode);
  }
}