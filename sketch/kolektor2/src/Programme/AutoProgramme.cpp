#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>
#include <TimeLib.h>
#include "../Constants/Constants.h"

#include "../Debugger/Debugger.h"
class AutoProgramme: public Programme {
  private:
    byte error = 0;
    Programme * actual;
    bool candidateSet = false;
    ProgrammeFactory * factory;
    Programme * _recuperation;
    Debugger * debugger;
    bool isInInterval(int days, int intervalStart, int intervalEnd) {
      if (intervalStart < intervalEnd) {
        return (intervalStart <= days) && (days < intervalEnd);
      } else {
        return (intervalStart < days) || (days <= intervalEnd);
      }
    }
    int daysSinceYearStart() {
      int elapsedDays = day();
      int monthNow = month();
      if (monthNow > 1) elapsedDays += 31;
      if (monthNow > 2) elapsedDays += 28;
      if (monthNow > 3) elapsedDays += 31;
      if (monthNow > 4) elapsedDays += 30;
      if (monthNow > 5) elapsedDays += 31;
      if (monthNow > 6) elapsedDays += 30;
      if (monthNow > 7) elapsedDays += 31;
      if (monthNow > 8) elapsedDays += 31;
      if (monthNow > 9) elapsedDays += 30;
      if (monthNow > 10) elapsedDays += 31;
      if (monthNow > 11) elapsedDays += 30;
      if (monthNow > 12) elapsedDays += 31;
      return elapsedDays;
    }

    void assignCandidate(ProgrammeContext* context) {
      if (!context->isTimeSet) {
        this->actual = this->factory->Disabled;
        this->candidateSet = true;
        return;
      }
      int elapsedDays = this->daysSinceYearStart();
      bool isInWinter = this->isInInterval(elapsedDays, context->data->autoWinterStart, context->data->autoWinterEnd);
      bool isInSummer = this->isInInterval(elapsedDays, context->data->autoSummerStart, context->data->autoSummerEnd);
      if (isInWinter) {
        this->actual = factory->Winter;  
      } else if (isInSummer) {
        this->actual = factory->Summer;
      } else {
        this->actual = factory->Recuperation;
      }
    }
  public:
    AutoProgramme(Debugger * debugger, ProgrammeFactory * factory) {
      this->debugger = debugger; 
      this->factory = factory;
      this->_recuperation = factory->Recuperation;
    }
    void onStart() {
      this->debugger->trace("Starting AUTO programme");
    }
    byte getCode() {
      if (this->error != 0) {
        return this->error;
      }
      if (this->candidateSet) {
        return this->actual->getCode();
      }
      return 130;
    }
    bool canForce() {
      return false;
    }
    bool handleClick(byte times) {
      return false;
    }
    int getDescription(char dest[80]) {
      if (this->error == 133) {
        strcpy(dest, "Čas není synchronizovaný");
        return 31;
      } else {
        char nested[40] = "";
        int written = this->actual->getDescription(nested);
        strcat(dest, "Auto mod: ");
        strcat(dest, nested);
        return 80;
      }
    }
    
    void configureTicking(RGBDiode * diode) {
      diode->configure(&tickingAutoProgrammeOn);
    }

    void getPower(ProgrammeContext* context, PowerOutput * out) {
      if (!context->isTimeSet) {
        this->debugger->trace("Time is not set, getting forecast");
        context->forecast->act(context->weatherDeps);
        this->error = 133;
        this->_recuperation->getPower(context, out);
        return;
      }
      this->assignCandidate(context);
      this->error = 0;
      this->actual->getPower(context, out);
    }
    void invalidate() {
      
    }
    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    bool isValid(ConfigurationData * data) {
      return data->mode == AUTO_MODE;
    }
};
