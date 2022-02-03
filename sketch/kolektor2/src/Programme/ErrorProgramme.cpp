#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>

class ErrorProgramme: public Programme {
  public:
    void onStart() {
      
    }
    int getDescription(char dest[80]) {
      strcpy(dest, "Chybná konfigurace");
      return 18;
    }
    byte getCode() {
      return 80;
    }
    boolean canForce() {
      return false;
    }
    boolean handleClick(short times) {
      return false;
    }
    void handleHold(int duration_ms, boolean finished) {

    }
    void configureTicking(RGBDiode * diode) {
      diode->configure(tickingCritical);
    }
    void getPower(ProgrammeContext* context, PowerOutput *out) {
      return;
    }
    boolean isValid(ConfigurationData * data) {
      return true;
    }
};