#include "src/Button/Button.h"
#include "src/PwmControl/PwmControl.h"
#include "src/Relay/Relay.h"
#include "src/DewPoint/DewPoint.h"
#include "src/RGBDiode/RGBDiode.h"

// PINS
#define PWM_1_CHANNEL 0
#define PWM_1_PIN 17
#define BTN_PIN 16
#define RELAY_PIN 15
#define RED_DIODE_PIN 14
#define GREEN_DIODE_PIN 13
#define BLUE_DIODE_PIN 12


class Handler: public ButtonHandler {
  boolean handleHold(int durationMillis, boolean finished) {
    return true;
  }
  int handleClick(byte clicks)  {
    
  }
};


Handler * buttonHandler = new Handler();

Button button(BTN_PIN, buttonHandler);
Relay relay(RELAY_PIN);
DewPoint dewPoint;
PwmControl pwm(PWM_1_CHANNEL, PWM_1_PIN);
RGBDiode diode(RED_DIODE_PIN, GREEN_DIODE_PIN, BLUE_DIODE_PIN);

void setup()
{
}

void loop() {
  dewPoint.compute(NAN, NAN);
  button.act();
  relay.act();
  delay(3000);
}
