#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>

class DisabledProgramme: public Programme {
  public:
    byte getCode() {
      return 90;
    }
    boolean canForce() {
      return false;
    }
    int getDescription(char dest[80]) {
      strcpy(dest, "Neaktivní mód");
      return 13;
    }
    void onStart() {
    }
    boolean handleClick(short times) {
      
    }
    void handleHold(int duration_ms, boolean finished) {

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
    boolean isValid(ConfigurationData* data) {
      return data->mode == INACTIVE_MODE;
    }
};
