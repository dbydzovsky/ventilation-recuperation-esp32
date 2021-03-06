#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"
#include "../Debugger/Debugger.h"
class TrialProgramme: public ConfigurableProgramme {
  private:
    bool valid = false;
    unsigned long started = 0;
    int durationMillis = 300000;
    PowerOutput output;
    Debugger * debugger;
  public:
    TrialProgramme(Debugger * debugger) {
      this->debugger = debugger;
    }
    byte getCode() {
      return 100;
    }
    bool canForce() {
      return true;
    }

    void onStart() {
      this->debugger->trace("Starting Trial programme");
      this->valid = true;
      this->started = millis();
    }

    void setPower(PowerOutput output, int duration) {
      this->output = output;
      this->durationMillis = duration;
      this->started = millis();
    }

    void invalidate() {
      this->valid = false;
    }

    bool handleClick(byte times) {
      this->valid = false;
      return false;
    }

    int getDescription(char dest[80]) {
      int waitingFor = millis() - this->started;
      int remaining = (this->durationMillis - waitingFor) / 1000;
      return sprintf(dest, "Manuální provoz. Zbývá %d sekund", remaining);
    }

    int remainingSeconds() {
      int waitingFor = millis() - this->started;
      return (this->durationMillis - waitingFor) / 1000;
    }

    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    void configureTicking(RGBDiode * diode) {
      diode->configure(&tickingTrial);
    }

    void getPower(ProgrammeContext* context, PowerOutput *out) {
      out->mode = this->output.mode;
      out->ventilatorPower = this->output.ventilatorPower;
      out->recuperationPower = this->output.recuperationPower;
      out->recuperationMode = this->output.recuperationMode;
    }
    PowerOutput getActualSetting() {
      return this->output;
    }
    int getDuration() {
      return this->durationMillis;
    }

    bool isValid(ConfigurationData * data) {
      if (!this->valid) {
        return false;
      }
      this->valid = millis() - this->started < this->durationMillis;
      return this->valid;
    }
};
