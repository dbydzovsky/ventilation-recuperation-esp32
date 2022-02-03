#include "src/Button/Button.h"
#include "src/PwmControl/PwmControl.h"
#include "src/Relay/Relay.h"
#include "src/DewPoint/DewPoint.h"
#include "src/RGBDiode/RGBDiode.h"
#include "src/Lock/Lock.h"
#include "src/Configuration/Configuration.h"
#include "src/Recuperation/Recuperation.h"
#include "src/Ventilator/Ventilator.h"
#include "src/Programme/Programme.h"
#include "src/Orchestrator/Orchestrator.h"

// PINS
#define PWM_1_CHANNEL 1
#define PWM_1_PIN 17
#define PWM_2_CHANNEL 2
#define PWM_2_PIN 18
#define BTN_PIN 16
#define RELAY_PIN 15
#define RED_DIODE_PIN 14
#define GREEN_DIODE_PIN 13
#define BLUE_DIODE_PIN 12

Relay * relay = new Relay(RELAY_PIN);
DewPoint * dewPoint = new DewPoint();
PwmControl * pwmVent = new PwmControl(PWM_1_CHANNEL, PWM_1_PIN);
PwmControl * pwmRecuperation = new PwmControl(PWM_2_CHANNEL, PWM_2_PIN);
RGBDiode * diode = new RGBDiode(RED_DIODE_PIN, GREEN_DIODE_PIN, BLUE_DIODE_PIN);
Lock * httpsLock = new Lock();
Lock * confLock = new Lock();
Configuration * configuration = new Configuration();
ProgrammeFactory * factory = new ProgrammeFactory();
Ventilator * ventilation = new Ventilator(pwmVent);
Recuperation * recuperation = new Recuperation(pwmRecuperation);

OrchestratorDependencies deps = {ventilation, recuperation, confLock, factory, diode, configuration->getData(), configuration};
Orchestrator * orchestrator = new Orchestrator(&deps);
class ButtonAdapter: public ButtonHandler {
  public:
    bool handleHold(int durationMillis, bool finished) {
        return orchestrator->handleHold(durationMillis, finished);
    }
    bool handleClick(byte times) {
        return orchestrator->handleClick(times);
    }
};
ButtonAdapter * adapter = new ButtonAdapter();
Button * button = new Button(BTN_PIN, adapter);

void setup()
{
  configuration->setup();

}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  dewPoint->compute(NAN, NAN);
  button->act();
  relay->enable();
  relay->act();
  relay->disable();
  delay(500);
  Programme * disabled = factory->Disabled;
  Programme * summer = factory->Summer;
  Programme * winter = factory->Winter;
  Programme * error = factory->Error;
  ConfigurableProgramme * trial = factory->Trial;
  Programme * inital = factory->Initial;
  Programme * autoProgramme = factory->Auto;
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
