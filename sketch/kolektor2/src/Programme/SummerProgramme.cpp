#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>


class SummerProgramme: public Programme {
  private:
    short currentRuleIndex = -1;
    byte error = 1;

    float markError(byte error) {
      this->error = error;
      return 0;
    }
  public:
    void onStart() {
    }
    byte getCode() {
      if (this->error != 0) {
        return this->error;
      }
      return 110;
    }
    bool canForce() {
      return false;
    }
    bool handleClick(byte times) {
      return false;
    }
    int getDescription(char dest[80]) {
      if (this->error == 111) {
        strcpy(dest, "Bylo dosáhnuto cílové nízké teploty");
        return 36;
      } else if (this->error == 113) {
        strcpy(dest, "Rosný bod překračuje vnitřní teplotu.");
        return 38;
      } else if (this->error == 114) {
        strcpy(dest, "Nelze spočítat rosný bod.");
        return 26;
      } else if (this->error == 115) {
        strcpy(dest, "Není k dispozici předpověď počasí.");
        return 34;
      } else if (this->error == 115) {
        strcpy(dest, "Není k dispozici předpověď počasí.");
        return 34;
      } else {
        strcpy(dest, "Chladící mód.");
        return 13;
      }
    }
    void configureTicking(RGBDiode *diode)  {
      if (this->error >= 113) {
        diode->configure(tickingConditionsMet);
      } else {
        if (WiFi.status() == WL_CONNECTED) {
          diode->configure(tickingSummerDeviceEnabledWifiOn);
        } else {
          diode->configure(tickingSummerDeviceEnabledWifiOff);
        }
      }
    }
    void getPower(ProgrammeContext *context, PowerOutput *out) {
      out->mode = POWER_OUTPUT_MODE_VENTILATION;
      
      context->forecast.act();
      if (!context->forecast.hasValidForecast()) {
        this->error = 115;
        return;
      }

      if (!context->forecast.shouldICool()) { 
        return;
      }
      
      if (context->tempInside < (context->data->summerMinInsideTemp / ((float)10))) {
        this->markError(111);
        return;
      }
    
      if (isnan(context->dewPoint)) {
        this->markError(114);
        return;
      }
      if (context->dewPoint > (context->tempInside - 2)) {
        this->markError(113);
        return;
      }
      
      Rules * rules = context->data->summerOnRules;
      for (short i = (rules->count - 1); i >= 0 ; i--) {
        Rule * rule = rules->rules[i];
        if (context->tempOutside < ((float) rule->temperature)) {
          this->currentRuleIndex = i;
          byte percentage = rule->percentage;
          this->error = 0;
          out->ventilatorPower = percentage;
          return;
        }
        if (this->currentRuleIndex == i) {
          if (context->tempOutside < (((float) rule->temperature) + temperatureDownTolerationProgramme)) {
            byte percentage = rule->percentage;
            this->error = 0;
            out->ventilatorPower = percentage;
            return;
          }
        }
      }
      this->currentRuleIndex = -1;
      this->error = 0;
      return;
    }
    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    bool isValid(ConfigurationData * data) {
      return data->mode == SUMMER_MODE;
    }
};
