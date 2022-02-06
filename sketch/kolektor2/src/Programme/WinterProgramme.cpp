#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>

class WinterProgramme: public Programme {
  private:
    Programme * _recuperation;
    short currentRuleIndex = -1;
    byte error = 0;
    float markError(byte error) {
      this->error = error;
      return 0;
    }
  public:
    WinterProgramme(Programme * Recuperation) {
      this->_recuperation = Recuperation;
    }
    void onStart() {
      this->currentRuleIndex = -1;
    }
    byte getCode() {
      if (this->error != 0) {
        return this->error;
      }
      return 120;
    }
    bool canForce() {
      return false;
    }
    bool handleClick(byte times) {
      return false;
    }
    int getDescription(char dest[80]) {
      if (this->error == 121) {
        strcpy(dest, "Teplota ve vnitřním prostoru byla překročena");
        return 45;
      } else if (this->error == 122) {
        strcpy(dest, "Teplota v kolektoru pomalu klesá.");
        return 34;
      } else if (this->error == 123) {
        strcpy(dest, "Rosný bod překračuje vnitřní teplotu.");
        return 36;
      } else if (this->error == 123) {
        strcpy(dest, "Rosný bod nelze spočítat.");
        return 26;
      } else {
        strcpy(dest, "Vyhřívací mód.");
        return 15;
      }
    }
    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    void configureTicking(RGBDiode * diode) {
      if (this->error != 0) {
        diode->configure(&tickingConditionsMet);
      } else {
        if (WiFi.status() == WL_CONNECTED) {
          diode->configure(&tickingDeviceEnabledWifiOn);
        } else {
          diode->configure(&tickingDeviceEnabledWifiOff);
        }
      }
    }
    void getPower(ProgrammeContext* context, PowerOutput* out) {
      out->mode = POWER_OUTPUT_MODE_VENTILATION;
      if (isnan(context->dewPoint)) {
        this->markError(124);
        return;
      }
      
      if (context->tempInside > context->data->winterMaxInsideTemp) {
        this->markError(121);
        this->_recuperation->getPower(context, out);
        return;
      }
      Rules * rules = context->data->winterOnRules;
      for (short i = (rules->count - 1); i >= 0; i--) {
        Rule * rule = rules->rules[i];
        if ((context->tempOutside) >= ((float) rule->targetValue)) {
          this->currentRuleIndex = i;
          this->error = 0;
          out->ventilatorPower = rule->percentage;
          if (rule->percentage == 0) {
            this->_recuperation->getPower(context, out);
          }
          return;
        }
        if (this->currentRuleIndex == i) {
          if ((context->tempOutside) > (((float) rule->targetValue) - temperatureDownTolerationProgramme)) {
            out->ventilatorPower = rule->percentage;
            this->error = 0;
            if (rule->percentage == 0) {
              this->_recuperation->getPower(context, out);
            }
            return;
          }
        }
      }
      this->_recuperation->getPower(context, out);
      this->currentRuleIndex = -1;
      this->error = 0;
      return;
    }
    
    void invalidate() {
      
    }
    bool isValid(ConfigurationData *data) {
      return data->mode == WINTER_MODE;
    }
};