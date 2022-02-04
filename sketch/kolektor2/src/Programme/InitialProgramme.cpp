#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>


class InitialWarmingUpProgramme: public Programme {
  private:
    unsigned long startedWaiting = NAN;
  public:
    byte getCode() {
      return 70;
    }
    bool canForce() {
      return true;
    }
    int getDescription(char dest[80]) {
      if (isnan(this->startedWaiting)) {
        return sprintf(dest, "Zahřívání...");
      } else {
        int waitingFor = millis() - this->startedWaiting;
        int remaining = (warmingUpTime - waitingFor) / 1000;
        return sprintf(dest, "Zahřívání... Zbývá %d sekund", remaining);
      }
    }
    void onStart() {
      this->startedWaiting = millis();
    }
    bool handleClick(byte times) {
      return false;
    }
    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    void configureTicking(RGBDiode *diode) {
      diode->configure(tickingWarmingUp);
    }
    void getPower(ProgrammeContext * context, PowerOutput * out) {
      return;
    }
    bool isValid(ConfigurationData * data) {
      if (isnan(this->startedWaiting)) {
        return false;
      }
      bool valid = millis() - this->startedWaiting < warmingUpTime;
      if (!valid) {
        this->startedWaiting = NAN;
      }
      return valid;
    }
};