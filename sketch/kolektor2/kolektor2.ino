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
#include "src/Settings/Settings.h"
#include "src/Restarter/Restarter.h"
#include "src/Display/Display.h"

HTTPClient httpClient;

// PINS
#define PWM_ventilator_PIN 16
#define ventilatorSignal 35
#define PWM_recuperation_PIN 33 // 26
#define recuperationSignal 13
#define BTN_PIN 23
#define RECUPERATION_RELAY_PIN 17
#define VENTILATOR_RELAY_PIN 27
#define RED_DIODE_PIN 39
#define GREEN_DIODE_PIN 19
#define BLUE_DIODE_PIN 24
#define rx_pin 26 // 16
#define tx_pin 18


RPMChecker * rpmVentilatorChecker = new RPMChecker(ventilatorSignal, "/blockvent");
RPMChecker * rpmRecuperationChecker = new RPMChecker(recuperationSignal, "/blockrecu");
int stateDiode = LED_BUILTIN;
DewPoint * dewPoint = new DewPoint();
#define PWM_1_CHANNEL 1
#define PWM_2_CHANNEL 2
#define PWM_3_CHANNEL 3
PwmControl * pwmVent = new PwmControl(PWM_1_CHANNEL, PWM_ventilator_PIN);
PwmControl * pwmRecuperation = new PwmControl(PWM_2_CHANNEL, PWM_recuperation_PIN);
RGBDiode * diode = new RGBDiode(PWM_3_CHANNEL, RED_DIODE_PIN, GREEN_DIODE_PIN, BLUE_DIODE_PIN);
Lock * httpsLock = new Lock();
Lock * confLock = new Lock();
Configuration * configuration = new Configuration();
ProgrammeFactory * factory = new ProgrammeFactory();

Relay * recuperationRelay = new Relay(RECUPERATION_RELAY_PIN);
Relay * ventilatorRelay = new Relay(VENTILATOR_RELAY_PIN);
Recuperation * recuperation = new Recuperation(recuperationRelay, pwmRecuperation, rpmRecuperationChecker);
Ventilator * ventilator = new Ventilator(ventilatorRelay, pwmVent, rpmVentilatorChecker);
Restarter * restarter = new Restarter();
Settings * settings = new Settings();
TimeProvider * timeProvider = new TimeProvider();
WeatherForecast * forecast = new WeatherForecast();


MHZ19_uart mhz19;

Sensors * sensors = new Sensors(&mhz19);
Average * outsideTemp = new Average(sensors->outsideTemp);
Average * outsideHum = new Average(sensors->outsideHum);
Average * insideTemp = new Average(sensors->insideTemp);
Average * insideHum = new Average(sensors->insideHum);
Average * co2Inside = new Average(sensors->co2Inside);

FilterMonitor * filter = new FilterMonitor(ventilator, recuperation);
Dependencies deps = { 
  ventilator, recuperation, confLock, httpsLock,
  factory, diode, configuration, 
  outsideTemp, outsideHum, insideTemp, insideHum, co2Inside, dewPoint,
  forecast, timeProvider, &httpClient,
  rpmVentilatorChecker, rpmRecuperationChecker, settings,
  restarter, filter
};
Orchestrator * orchestrator = new Orchestrator(&deps);
Monitoring * monitoring = new Monitoring(orchestrator, &deps);
Display * display = new Display(&deps, orchestrator);
Button * button = new Button(BTN_PIN, display);

DNSServer dns;
AsyncWebServer server(80);
AsyncWiFiManager wifiManager(&server, &dns);
HttpServer * httpServer = new HttpServer(&deps, &server, &wifiManager, orchestrator, filter);

long ventilatorTicks = 0;
void IRAM_ATTR myVentilatorHandler() {
  ventilatorTicks = ventilatorTicks + 1;
}
void attachVentilator() {
  attachInterrupt(digitalPinToInterrupt(ventilatorSignal), myVentilatorHandler, RISING);
}
void detachVentilator() {
  detachInterrupt(digitalPinToInterrupt(ventilatorSignal));
}

long recuperationTicks = 0;
void IRAM_ATTR myRecuperationHandler() {
  recuperationTicks = recuperationTicks + 1;
}

void attachRecuperation() {
  attachInterrupt(digitalPinToInterrupt(recuperationSignal), myRecuperationHandler, RISING);
}
void detachRecuperation() {
  detachInterrupt(digitalPinToInterrupt(recuperationSignal));
}


void setup()
{
  pinMode(stateDiode, OUTPUT);
  digitalWrite(stateDiode, HIGH);
  mhz19.begin(rx_pin, tx_pin);
  mhz19.setAutoCalibration(true);
  display->setup();
  Serial.begin(9600);
  SPIFFS.begin();
  delay(2000);
  settings->setup();
  httpClient.setReuse(true);
  digitalWrite(stateDiode, LOW);
  configuration->setup();
  display->wifiConnecting();
  httpServer->setup();
  sensors->setup();
  filter->setup();
  rpmVentilatorChecker->setup();
  rpmRecuperationChecker->setup();
  attachRecuperation();

  SettingsData * settingsData = settings->getSettings();
  if (!settingsData->checkRecuperationRpm){ 
    rpmRecuperationChecker->deactivate();
  } else {
    attachVentilator();
  }
  if (!settingsData->checkVentilatorRpm){ 
    rpmVentilatorChecker->deactivate();
  } else {
    attachRecuperation();  
  }
  if (settingsData->hideCo2) {
    monitoring->hideCo2();
  }
  if (settingsData->hideInternalTempHum) {
    monitoring->hideInternalTempHum();  
  }
  rpmRecuperationChecker->setUnblockingFansPeriod(settingsData->unblockingFansPeriod);
  rpmRecuperationChecker->setMaxRpm(settingsData->recuperationMaxRpm);
  rpmVentilatorChecker->setUnblockingFansPeriod(settingsData->unblockingFansPeriod);
  rpmVentilatorChecker->setMaxRpm(settingsData->ventilatorMaxRpm);
  recuperation->setDurationChangeWait(settingsData->recuperationWaitForDirectionChange);
  recuperation->setCycleDuration(settingsData->recuperationCycleDuration); 
  diode->setBrightness(settingsData->brightness);
}

unsigned long last_sensor_reading = millis();
#define averageReadingInterval 2000

void loop() {
  SettingsData * settingsData = settings->getSettings();
  delay(10);
  display->act();
  restarter->act();
  if (millis() - last_sensor_reading > averageReadingInterval) {
    last_sensor_reading = millis();
    outsideTemp->doReading();
    outsideHum->doReading();
    co2Inside->doReading();
    insideTemp->doReading();
    insideHum->doReading();
    dewPoint->compute(outsideHum->getAverage(), outsideTemp->getAverage());
  }
  monitoring->act();
  button->act();
  ventilator->act();
  recuperation->act();
  recuperationRelay->act();
  ventilatorRelay->act();
  dns.processNextRequest();
  orchestrator->act();
  filter->act();
  if (settingsData->checkVentilatorRpm){
    if (rpmVentilatorChecker->act(ventilatorTicks, ventilator->getPower())) {
      detachVentilator();
      ventilatorTicks = 0;
      attachVentilator();  
    }
  }
  if (settingsData->checkRecuperationRpm){
    if (rpmRecuperationChecker->act(recuperationTicks, recuperation->getActualPower())) {
      detachRecuperation();
      recuperationTicks = 0;
      attachRecuperation();  
    }
  }

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
