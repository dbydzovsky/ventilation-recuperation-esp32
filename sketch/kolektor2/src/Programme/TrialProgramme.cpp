#include "Arduino.h"
#include "Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include <WiFiClientSecure.h>

class TrialProgramme: public ConfigurableProgramme {
  private:
    boolean valid = false;
    unsigned long started = 0;
    int durationMillis = 60000;
    PowerOutput output;
  public:
    byte getCode() {
      return 100;
    }
    boolean canForce() {
      return true;
    }

    void onStart() {
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

    boolean handleClick(short times) {
      this->valid = false;
      return false;
    }

    int getDescription(char dest[80]) {
      int waitingFor = millis() - this->started;
      int remaining = (this->durationMillis - waitingFor) / 1000;
      return sprintf(dest, "Manuální provoz. Zbývá %d sekund", remaining);
    }

    void handleHold(int duration_ms, boolean finished) {

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

    boolean isValid(ConfigurationData * data) {
      if (!this->valid) {
        return false;
      }
      this->valid = millis() - this->started < this->durationMillis;
      return this->valid;
    }
};
