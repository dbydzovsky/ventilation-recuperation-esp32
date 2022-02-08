#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"

class InitialWarmingUpProgramme: public Programme {
  private:
    unsigned long startedWaiting;
    bool _isValid = false;
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
      if (IS_DEBUG) Serial.println("Starting Initial Programme");
      this->startedWaiting = millis();
      this->_isValid = true;
    }
    bool handleClick(byte times) {
      return false;
    }
    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    void configureTicking(RGBDiode *diode) {
      diode->configure(&tickingWarmingUp);
    }
    void getPower(ProgrammeContext * context, PowerOutput * out) {
      return;
    }
    void invalidate() {
      this->_isValid = false;
    }
    bool isValid(ConfigurationData * data) {
      if (!this->_isValid) {
        return false;
      }
      bool valid = millis() - this->startedWaiting < warmingUpTime;
      if (!valid) {
        this->_isValid = false;
      }
      return valid;
    }
};