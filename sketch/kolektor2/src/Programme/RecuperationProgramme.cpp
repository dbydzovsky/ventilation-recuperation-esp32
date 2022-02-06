#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"

class RecuperationProgramme: public Programme {
  private:
    bool directionIn = false;
    short currentRuleIndex = -1;
    byte error = 0;
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
      if (!isnan(co2Value)) {
        Rules * rules = context->data->co2Rules;
        for (short i = (rules->count - 1); i >= 0; i--) {
          Rule * rule = rules->rules[i];
          if ((co2Value) >= ((float) rule->targetValue)) {
            this->currentRuleIndex = i;
            this->error = 0;
            out->recuperationPower = rule->percentage;
            return;
          }
          if (this->currentRuleIndex == i) {
            if ((co2Value) > (((float) rule->targetValue) - co2DownTolerationProgramme)) {
              out->recuperationPower = rule->percentage;
              this->error = 0;
              return;
            }
          }
        }
        this->currentRuleIndex = -1;
        this->error = 0;
      }
    }
    void invalidate() {
      
    }
    bool isValid(ConfigurationData * data) {
      return true;
    }
};