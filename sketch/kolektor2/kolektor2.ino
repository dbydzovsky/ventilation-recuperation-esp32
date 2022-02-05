#include "SPIFFS.h"
#include <HTTPClient.h>
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
#include "src/Average/Average.h"
#include "src/Sensors/Sensors.h"
#include "src/Dependencies/Dependencies.h"
#include "src/Weather/Weather.h"
#include "src/TimeProvider/TimeProvider.h"
#include "src/Button/Button.h"

HTTPClient httpClient;

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


DewPoint * dewPoint = new DewPoint();
PwmControl * pwmVent = new PwmControl(PWM_1_CHANNEL, PWM_1_PIN);
PwmControl * pwmRecuperation = new PwmControl(PWM_2_CHANNEL, PWM_2_PIN);
RGBDiode * diode = new RGBDiode(RED_DIODE_PIN, GREEN_DIODE_PIN, BLUE_DIODE_PIN);
Lock * httpsLock = new Lock();
Lock * confLock = new Lock();
Configuration * configuration = new Configuration();
ProgrammeFactory * factory = new ProgrammeFactory();
Ventilator * ventilator = new Ventilator(pwmVent);
Relay * relay = new Relay(RELAY_PIN);
Recuperation * recuperation = new Recuperation(relay, pwmRecuperation);

TimeProvider * timeProvider = new TimeProvider();
WeatherForecast * forecast = new WeatherForecast();

Sensors * sensors = new Sensors();
Average * outsideTemp = new Average(sensors->outsideTemp);
Average * outsideHum = new Average(sensors->outsideHum);
Average * insideTemp = new Average(sensors->insideTemp);
Average * co2Inside = new Average(sensors->co2Inside);

Dependencies deps = { 
  ventilator, recuperation, confLock, httpsLock,
  factory, diode, configuration, 
  outsideTemp, outsideHum, insideTemp, co2Inside,
  forecast, timeProvider, &httpClient 
};
Orchestrator * orchestrator = new Orchestrator(&deps);


Button * button = new Button(BTN_PIN, orchestrator);
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);
  SPIFFS.begin();
  delay(500);
  httpClient.setReuse(true);
  digitalWrite(LED_BUILTIN, LOW);
  configuration->setup();
}

unsigned long last_sensor_reading = millis();
#define averageReadingInterval 2000

void loop() {
  Serial.println("loop start");
  digitalWrite(LED_BUILTIN, LOW);
  if (millis() - last_sensor_reading > averageReadingInterval) {
    last_sensor_reading = millis();
    outsideTemp->doReading();
    outsideHum->doReading();
    co2Inside->doReading();
    insideTemp->doReading();
    dewPoint->compute(outsideHum->getValue(), outsideTemp->getValue());
  }
  button->act();
  ventilator->act();
  recuperation->act();
  relay->act();
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  orchestrator->act();
  Serial.println(ESP.getFreeHeap());
}
