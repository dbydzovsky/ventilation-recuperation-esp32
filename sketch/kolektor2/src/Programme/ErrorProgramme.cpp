#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"
#include "../Debugger/Debugger.h"

class ErrorProgramme: public Programme {
  private:
    Debugger * debugger;
  public:
    ErrorProgramme (Debugger * debugger) {
      this->debugger = debugger;
    }
    void onStart() {
      this->debugger->trace("Starting Error Programme");
    }
    int getDescription(char dest[80]) {
      strcpy(dest, "Chybná konfigurace");
      return 18;
    }
    byte getCode() {
      return 80;
    }
    bool canForce() {
      return false;
    }
    bool handleClick(byte times) {
      return false;
    }
    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    void configureTicking(RGBDiode * diode) {
      diode->configure(&tickingCritical);
    }
    void getPower(ProgrammeContext* context, PowerOutput *out) {
      return;
    }
    void invalidate() {
      
    }
    bool isValid(ConfigurationData * data) {
      return true;
    }
};