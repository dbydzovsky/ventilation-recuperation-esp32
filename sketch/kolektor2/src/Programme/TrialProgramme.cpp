#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"
class TrialProgramme: public ConfigurableProgramme {
  private:
    bool valid = false;
    unsigned long started = 0;
    int durationMillis = 60000;
    PowerOutput output;
  public:
    byte getCode() {
      return 100;
    }
    bool canForce() {
      return true;
    }

    void onStart() {
      if (IS_DEBUG)Serial.println("Starting Trial programme");
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

    bool handleHold(int duration_ms, bool finished) {
      return false;
    }
    void configureTicking(RGBDiode * diode) {
      diode->configure(tickingTrial);
    }

    void getPower(ProgrammeContext* context, PowerOutput *out) {
      out->mode = this->output.mode;
      out->ventilatorPower = this->output.ventilatorPower;
      out->recuperationPower = this->output.recuperationPower;
      out->recuperationMode = this->output.recuperationMode;
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
