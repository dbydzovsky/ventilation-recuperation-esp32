
#include "SPIFFS.h"
#include <HTTPClient.h>

#include "AsyncJson.h"
#include <WiFiUdp.h>
#include <ESPAsyncWebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <MHZ19_uart.h> // https://github.com/nara256/mhz19_uart

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
#include "src/Monitoring/Monitoring.h"
#include "src/HttpServer/HttpServer.h"
#include "src/Constants/Constants.h"
#include "src/FilterMonitor/FilterMonitor.h"
#include "src/RPMChecker/RPMChecker.h"

HTTPClient httpClient;

// PINS
#define PWM_ventilator_PIN 5
#define ventilatorSignal 18
#define PWM_recuperation_PIN 14
#define recuperationSignal 19
#define BTN_PIN 15
#define RELAY_PIN 13
#define RED_DIODE_PIN 12
#define GREEN_DIODE_PIN 3
#define BLUE_DIODE_PIN 4
#define rx_pin 16
#define tx_pin 17


RPMChecker * rpmVentilatorChecker = new RPMChecker(ventilatorSignal, VENTILATOR_MAX_RPM, "/blockvent");
RPMChecker * rpmRecuperationChecker = new RPMChecker(recuperationSignal, RECUPERATION_MAX_RPM, "/blockrecu");
int stateDiode = LED_BUILTIN;
DewPoint * dewPoint = new DewPoint();
#define PWM_1_CHANNEL 1
#define PWM_2_CHANNEL 2
PwmControl * pwmVent = new PwmControl(PWM_1_CHANNEL, PWM_ventilator_PIN);
PwmControl * pwmRecuperation = new PwmControl(PWM_2_CHANNEL, PWM_recuperation_PIN);
RGBDiode * diode = new RGBDiode(RED_DIODE_PIN, GREEN_DIODE_PIN, BLUE_DIODE_PIN);
Lock * httpsLock = new Lock();
Lock * confLock = new Lock();
Configuration * configuration = new Configuration();
ProgrammeFactory * factory = new ProgrammeFactory();
Ventilator * ventilator = new Ventilator(pwmVent, rpmVentilatorChecker);
Relay * relay = new Relay(RELAY_PIN);
Recuperation * recuperation = new Recuperation(relay, pwmRecuperation, rpmRecuperationChecker);

TimeProvider * timeProvider = new TimeProvider();
WeatherForecast * forecast = new WeatherForecast();


MHZ19_uart mhz19;

Sensors * sensors = new Sensors(&mhz19);
Average * outsideTemp = new Average(sensors->outsideTemp);
Average * outsideHum = new Average(sensors->outsideHum);
Average * insideTemp = new Average(sensors->insideTemp);
Average * co2Inside = new Average(sensors->co2Inside);

Dependencies deps = { 
  ventilator, recuperation, confLock, httpsLock,
  factory, diode, configuration, 
  outsideTemp, outsideHum, insideTemp, co2Inside, dewPoint,
  forecast, timeProvider, &httpClient,
  rpmVentilatorChecker, rpmRecuperationChecker
};
Orchestrator * orchestrator = new Orchestrator(&deps);
Monitoring * monitoring = new Monitoring(orchestrator, &deps);
FilterMonitor * filter = new FilterMonitor(ventilator, recuperation);
Button * button = new Button(BTN_PIN, orchestrator);

DNSServer dns;
AsyncWebServer server(80);
AsyncWiFiManager wifiManager(&server, &dns);
HttpServer * httpServer = new HttpServer(&deps, &server, &wifiManager, orchestrator, filter);
void setup()
{
  pinMode(stateDiode, OUTPUT);
  digitalWrite(stateDiode, HIGH);
  Serial.begin(9600);
  SPIFFS.begin();
  delay(2000);
  
  Serial2.begin(9600, SERIAL_8N1, rx_pin, tx_pin);
  mhz19.begin(rx_pin, tx_pin);
  mhz19.setAutoCalibration(true);
  
  httpClient.setReuse(true);
  digitalWrite(stateDiode, LOW);
  configuration->setup();
  httpServer->setup();
  filter->setup();
  rpmVentilatorChecker->setup();
  rpmRecuperationChecker->setup();
}

unsigned long last_sensor_reading = millis();
#define averageReadingInterval 2000

void loop() {
  if (millis() - last_sensor_reading > averageReadingInterval) {
    digitalWrite(stateDiode, HIGH);
    delay(15);
    digitalWrite(stateDiode, LOW);
    last_sensor_reading = millis();
    outsideTemp->doReading();
    outsideHum->doReading();
    co2Inside->doReading();
    insideTemp->doReading();
    dewPoint->compute(outsideHum->getAverage(), outsideTemp->getAverage());
  }
  monitoring->act();
  if (!IS_DEBUG) {
    button->act();
  }
  ventilator->act();
  recuperation->act();
  relay->act();
  dns.processNextRequest();
  orchestrator->act();
  filter->act();
  rpmVentilatorChecker->act(ventilator->getPower());
  rpmRecuperationChecker->act(recuperation->getPower());
  delay(30);
  
  if (IS_DEBUG) {
    if (Serial.available() > 0){ 
      String command = Serial.readStringUntil(':');
      if (command == "ti") {
        float value = Serial.parseFloat();
        Serial.print("Setting inside temperature with: ");
        Serial.println(value);
        sensors->insideTemp->setValue(value);
      } else if (command == "to") {
        float value = Serial.parseFloat();
        Serial.print("Setting outside temperature with: ");
        Serial.println(value);
        sensors->outsideTemp->setValue(value);
      }else if (command == "ho") {
        float value = Serial.parseFloat();
        Serial.print("Setting outside humidity with: ");
        Serial.println(value);
        sensors->outsideHum->setValue(value);
      } else if (command == "co2") {
        float value = Serial.parseFloat();
        Serial.print("Setting co2 inside with: ");
        Serial.println(value);
        sensors->co2Inside->setValue(value);
      } else if (command == "help" || command == "h") {
        Serial.println("to (temperature outside): xxxx.xx");
        Serial.println("ho (humidity outside): xxxx.xx");
        Serial.println("ti (temperature inside): xxxx.xx");
        Serial.println("co2 (co2 inside): xxxx.xx");
      }
    }
  }
}
