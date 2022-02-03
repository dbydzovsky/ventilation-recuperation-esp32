
#include <analogWrite.h>


#define pinPwm 17
#define freq 10000
#define pwmChannel 0
#define resolution 8

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(pinPwm, OUTPUT);
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(pinPwm, pwmChannel);
}


void loop() {
  for(int dutyCycle = 100; dutyCycle <= 255; dutyCycle = dutyCycle + 20){   
    // changing the LED brightness with PWM
    ledcWrite(pwmChannel, dutyCycle);
    delay(2000);
  }
}
