#include "Arduino.h"
#include "Orchestrator.h"
#include "../Configuration/Configuration.h"
#include "../Constants/Constants.h"
#include "../Button/Button.h"
#include "../Dependencies/Dependencies.h"
#include "../Weather/Weather.h"
#include "../Settings/Settings.h"

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
  this->actual = deps->factory->Initial;  
}
int Orchestrator::getProgrammeCode() {
  return this->_programCode;
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
  this->deps->debugger->debug("Assigning new Programme");
  Programme * candidate = this->deps->factory->Disabled;
  if (this->deps->confLock->readLock()) {
    if (!this->deps->conf->dataSet) {
      this->deps->debugger->debug("Data not set. Error programme configured.");
      candidate = this->deps->factory->Error;
    } else if (this->deps->conf->getData()->mode == WINTER_MODE) {
      this->deps->debugger->debug("Assinning Winter programme");
      candidate = this->deps->factory->Winter;
    } else if (this->deps->conf->getData()->mode == SUMMER_MODE) {
      this->deps->debugger->debug("Assigning Summer programme");
      candidate = this->deps->factory->Summer;
    } else if (this->deps->conf->getData()->mode == AUTO_MODE) {
      this->deps->debugger->debug("Assigning Auto programme");  
      candidate = this->deps->factory->Auto;
    } else {
      this->deps->debugger->debug("Assigning Disabled programme");  
      candidate = this->deps->factory->Disabled;
    }
    this->deps->confLock->readUnlock();
  }
  if (this->actual != candidate) {
    this->deps->debugger->debug("Actual programme has changed.");
    this->actual->invalidate();
    this->actual = candidate;
    this->actual->onStart();
  }
}

bool Orchestrator::handleClick(byte times) {
  if (times == 2) {
    ConfigurableProgramme * trialProgramme = this->deps->factory->Trial;
    PowerOutput output;
    SettingsData * settings = this->deps->settings->getSettings();
    trialProgramme->setPower(output, settings->tempDisableDuration);
    this->setProgramme(dynamic_cast<Programme*>(trialProgramme));
    return true;
  }
  if (times == 3) {
    ConfigurableProgramme * trialProgramme = this->deps->factory->Trial;
    PowerOutput output;
    
    SettingsData * settings = this->deps->settings->getSettings();
    output.mode = POWER_OUTPUT_MODE_BOTH;
    output.ventilatorPower = settings->ventilatorPower;    
    output.recuperationPower = settings->recuperationPower;
    output.recuperationMode = settings->recuperationMode;
    trialProgramme->setPower(output, settings->durationMillis);
    this->setProgramme(dynamic_cast<Programme*>(trialProgramme));
    return true;
  }
  return this->actual->handleClick((byte)times);
}

bool Orchestrator::handleHold(int duration_ms, bool finished) {
  if (this->actual->handleHold(duration_ms, finished)) {
    return true;
  }
  if (finished) {
    this->deps->diode->detach();
  }
  return finished;
}

int Orchestrator::validateCommonSense() {
  float outsideAirTemp = this->deps->outsideTemp->getAverage();
  if (isnan(outsideAirTemp)) {
    return 51;
  }
  float insideTemp = this->deps->insideTemp->getAverage();
  if (isnan(insideTemp)) {
    return 52;
  }
  float outsideHumidity = this->deps->outsideHum->getAverage();
  if (isnan(outsideHumidity)) {
    return 53;
  }
  return 0;
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
  context.dewPointOut = this->deps->dewPointOut->getDewPoint();
  context.dewPointIn = this->deps->dewPointIn->getDewPoint();
  context.humidityOutside = this->deps->outsideHum->getAverage();
  context.tempInside = this->deps->insideTemp->getAverage();
  context.tempOutside = this->deps->outsideTemp->getAverage();
  context.co2 = this->deps->co2Inside->getAverage();
  context.isTimeSet = this->deps->timeProvider->isTimeSet();
  context.data = data;
  context.forecast = this->deps->forecast;
  context.settings = this->deps->settings;
  WeatherDeps weatherDeps = {data, this->deps->timeProvider, this->deps->httpClient, this->deps->httpLock, this->deps->debugger};
  context.weatherDeps = &weatherDeps;
  PowerOutput out;
  if (this->actual->canForce()) {
    this->actual->configureTicking(this->deps->diode);
    this->actual->getPower(&context, &out);
    this->_programCode = this->actual->getCode();
  } else {
    this->_programCode = this->validateCommonSense();
    if (this->_programCode == 0) {
      this->actual->configureTicking(this->deps->diode);
      this->actual->getPower(&context, &out);
      this->_programCode = this->actual->getCode();
    } else {
      this->deps->diode->configure(&tickingError);
    }
  }
  this->deps->confLock->readUnlock();
  this->deps->ventilation->setPower(out.ventilatorPower);
  this->deps->recuperation->setPower(out.recuperationPower, out.recuperationMode);
}