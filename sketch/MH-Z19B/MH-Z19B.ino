
#include <SoftwareSerial.h>
#include <MHZ.h>

// pin for pwm reading
#define CO2_IN 36

// pin for uart reading
#define MH_Z19_RX 17  // D7
#define MH_Z19_TX 5  // D6

MHZ co2(MH_Z19_RX, MH_Z19_TX, CO2_IN, MHZ19B);
int readCO2PWM() {
  // todo use interrupts
  // https://roboticsbackend.com/arduino-pulsein-with-interrupts/
  unsigned long th, tl, ppm_pwm = 0;
  do {
    th = pulseIn(CO2_IN, HIGH, 1004000) / 1000;
    tl = 1004 - th;
    ppm_pwm = 5000 * (th-2)/(th+tl-4);
  } while (th == 0);
  return ppm_pwm;  
}
void setup() {
  Serial.begin(9600);
  pinMode(CO2_IN, INPUT);
  delay(1000);
  Serial.println("MHZ 19B");

}

void loop() {
  Serial.print("----- Time from start: ");
  Serial.print(millis() / 1000);
  Serial.println(" s");

  Serial.print("PWM: ");
  int ppm_pwm = co2.readCO2PWM();
  Serial.println(ppm_pwm);
  
  //Serial.print("Pulse In: ");
  //int ppm_pulse = readCO2PWM();
  //Serial.println(ppm_pulse);
  
  Serial.println("------------------------------");
  delay(5000);
}
