#include "Arduino.h"
#include "Orchestrator.h"
#include "../Configuration/Configuration.h"
#include "../Constants/Constants.h"
#include "../Button/Button.h"
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

Orchestrator::Orchestrator(OrchestratorDependencies * deps) {
  this->deps = deps;
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
  Programme * candidate = this->deps->factory->Disabled;
  if (this->deps->lock->readLock()) {
    byte mode = this->deps->data->mode;
    if (!this->deps->conf->dataSet) {
      candidate = this->deps->factory->Error;
    } else if (mode == WINTER_MODE) {
      candidate = this->deps->factory->Winter;
    } else if (mode == SUMMER_MODE) {
      candidate = this->deps->factory->Summer;
    } else if (mode == AUTO_MODE) {
      candidate = this->deps->factory->Auto;
    } else {
      candidate = this->deps->factory->Disabled;
    }
    this->deps->lock->readUnlock();
  }
  if (this->actual != candidate) {
    this->actual = candidate;
    this->actual->onStart();
  }
}

bool Orchestrator::handleClick(byte times) {
  if (times == 2) {
    ConfigurableProgramme * trialProgramme = this->deps->factory->Trial;
    PowerOutput output;
    trialProgramme->setPower(output, temporaryDisabledProgrammeDuration);
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
  return this->actual->handleClick(times);
}

bool Orchestrator::handleHold(int duration_ms, bool finished) {
  this->actual->handleHold(duration_ms, finished);
  if (duration_ms > 10000) {
    // todo restart();
  }
  if (finished) {
    // todo diode->setPriority(0);
    if (duration_ms > enablementTresholdMs) {
      if (this->deps->conf->dataSet) {
        // todo configuration->changeProperty("mo", INACTIVE_MODE);
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
  if (!this->deps->lock->readLock()) {
    return;
  }
  ConfigurationData * data = this->deps->data;
  if (!this->actual->isValid(data)) {
    this->assignProgramme();
  }
  ProgrammeContext context;
  context.dewPoint = 5;
  context.humidityOutside = 60;
  context.tempInside = 20;
  context.tempOutside = 60;
  context.isTimeSet = false;
  context.data = data;

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
      this->actual->getPower(&context, &out);
      programCode = this->actual->getCode();
    } else {
      this->deps->diode->configure(tickingError);
    }
  }
  this->deps->lock->readUnlock();
  if (out.mode == POWER_OUTPUT_MODE_VENTILATION || out.mode == POWER_OUTPUT_MODE_BOTH) {
    this->deps->ventilation->setPower(out.ventilatorPower);
  }
  if (out.mode == POWER_OUTPUT_MODE_RECUPERATION || out.mode == POWER_OUTPUT_MODE_BOTH) {
    this->deps->recuperation->setPower(out.recuperationPower, out.recuperationMode);
  }
}