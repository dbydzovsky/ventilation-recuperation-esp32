#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"

class DisabledProgramme: public Programme {
  public:
    byte getCode() {
      return 90;
    }
    bool canForce() {
      return false;
    }
    int getDescription(char dest[80]) {
      strcpy(dest, "Neaktivní mód");
      return 13;
    }
    void onStart() {
      if (IS_DEBUG) Serial.println("Starting Disabled Programme");
    }
    bool handleClick(byte times) {
      
    }
    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    void configureTicking(RGBDiode *diode) {
      if (WiFi.status() == WL_CONNECTED) {
        diode->configure(&tickingDeviceDisabledWifiOn);
      } else {
        diode->configure(&tickingDeviceDisabledWifiOff);
      }
    } 
    void getPower(ProgrammeContext * context, PowerOutput *out) {
      return;
    }
    bool isValid(ConfigurationData* data) {
      return data->mode == INACTIVE_MODE;
    }
};
