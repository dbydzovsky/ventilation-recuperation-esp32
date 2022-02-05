#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"

class RecuperationProgramme: public Programme {
  private:
    bool directionIn = false;
  public:
    byte getCode() {
      return 200;
    }
    bool canForce() {
      return false;
    }
    int getDescription(char dest[80]) {
      return sprintf(dest, "Řízené větrání...");
    }
    void onStart() {
      if (IS_DEBUG) Serial.println("Starting Recuperation Programme");
    }
    bool handleClick(byte times) {
      return false;
    }
    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    void configureTicking(RGBDiode *diode) {
      diode->configure(&tickingRecuperation);
    }
    void getPower(ProgrammeContext * context, PowerOutput * out) {
      if (out->mode == POWER_OUTPUT_MODE_VENTILATION) {
        out->mode = POWER_OUTPUT_MODE_BOTH;
      } else {
        out->mode = POWER_OUTPUT_MODE_RECUPERATION;
      }
      out->recuperationMode = RECUPERATION_MODE_RECYCLE;
      float co2Value = context->co2;
      // todo configuration
      if (co2Value > 700) {
        out->recuperationPower = 100;
      }
    }
    void invalidate() {
      
    }
    bool isValid(ConfigurationData * data) {
      return true;
    }
};