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
#include "src/Debugger/Debugger.h"

TimeProvider * timeProvider = new TimeProvider();
Debugger * debugger = new Debugger(timeProvider);
HTTPClient httpClient;

// PINS
#define PWM_ventilator_PIN 23
#define ventilatorSignal 34
#define PWM_recuperation_PIN 18
#define recuperationSignal 33
#define BTN_PIN 26 // see GPIO_NUM_X when changing
#define RECUPERATION_RELAY_PIN 19
#define VENTILATOR_RELAY_PIN 17
#define RED_DIODE_PIN 39 // not-used
#define GREEN_DIODE_PIN 19 // not-used
#define BLUE_DIODE_PIN 24 // not-used
#define rx_pin 36
#define tx_pin 5


RPMChecker * rpmVentilatorChecker = new RPMChecker(ventilatorSignal, "/blockvent", debugger);
RPMChecker * rpmRecuperationChecker = new RPMChecker(recuperationSignal, "/blockrecu", debugger);
int stateDiode = LED_BUILTIN;
DewPoint * dewPointOut = new DewPoint();
DewPoint * dewPointIn = new DewPoint();
#define PWM_1_CHANNEL 1
#define PWM_2_CHANNEL 2
#define PWM_3_CHANNEL 3
PwmControl * pwmVent = new PwmControl(PWM_1_CHANNEL, PWM_ventilator_PIN);
PwmControl * pwmRecuperation = new PwmControl(PWM_2_CHANNEL, PWM_recuperation_PIN);
RGBDiode * diode = new RGBDiode(PWM_3_CHANNEL, RED_DIODE_PIN, GREEN_DIODE_PIN, BLUE_DIODE_PIN);
Lock * httpsLock = new Lock();
Lock * confLock = new Lock();
Configuration * configuration = new Configuration(debugger);
ProgrammeFactory * factory = new ProgrammeFactory(debugger);

Relay * recuperationRelay = new Relay(debugger, RECUPERATION_RELAY_PIN);
Relay * ventilatorRelay = new Relay(debugger, VENTILATOR_RELAY_PIN);
Recuperation * recuperation = new Recuperation(recuperationRelay, pwmRecuperation, rpmRecuperationChecker);
Ventilator * ventilator = new Ventilator(ventilatorRelay, pwmVent, rpmVentilatorChecker, debugger);

Settings * settings = new Settings(debugger);
WeatherForecast * forecast = new WeatherForecast();

  
MHZ19_uart mhz19;

Sensors * sensors = new Sensors(&mhz19);
Average * outsideTemp = new Average(sensors->outsideTemp);
Average * outsideHum = new Average(sensors->outsideHum);
Average * insideTemp = new Average(sensors->insideTemp);
Average * insideHum = new Average(sensors->insideHum);
Average * co2Inside = new Average(sensors->co2Inside);

FilterMonitor * filter = new FilterMonitor(ventilator, recuperation, debugger);
Restarter * restarter = new Restarter(filter);
Dependencies deps = { 
  ventilator, recuperation, confLock, httpsLock,
  factory, diode, configuration, 
  outsideTemp, outsideHum, insideTemp, insideHum, co2Inside, dewPointOut, dewPointIn, 
  forecast, timeProvider, &httpClient,
  rpmVentilatorChecker, rpmRecuperationChecker, settings,
  restarter, filter, debugger
};
  
Orchestrator * orchestrator = new Orchestrator(&deps);
Monitoring * monitoring = new Monitoring(orchestrator, &deps);
DNSServer dns;
AsyncWebServer server(80);
AsyncWiFiManager wifiManager(&server, &dns);

HttpServer * httpServer = new HttpServer(&deps, &server, &wifiManager, orchestrator, filter);
Display * display = new Display(&deps, orchestrator, httpServer);
Button * button = new Button(BTN_PIN, display);

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
  
  SettingsData * settingsData = settings->getSettings();
  
  rpmRecuperationChecker->setTicksPerRevolution(settingsData->recuperationRevolutions);
  if (!settingsData->checkRecuperationRpm || !settingsData->recuperationOn){ 
    rpmRecuperationChecker->deactivate();
  } else {
    attachRecuperation();
  }
  
  rpmVentilatorChecker->setTicksPerRevolution(settingsData->ventilatorRevolutions);
  rpmVentilatorChecker->setMaxTemperature(settingsData->maxVentilatorTemp);  
  if (!settingsData->checkVentilatorRpm){ 
    rpmVentilatorChecker->deactivate();
  } else {
    attachVentilator();  
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
  debugger->setTrace(false);
}

unsigned long last_sensor_reading = millis();
#define averageReadingInterval 2000
unsigned long co2_last_sensor_reading = millis();
#define averageCo2ReadingInterval 5000

void loop() {
  SettingsData * settingsData = settings->getSettings();
  delay(10);
  display->act();
  restarter->act();

  if (millis() - co2_last_sensor_reading > averageCo2ReadingInterval) {
    co2_last_sensor_reading = millis();
    co2Inside->doReading();
  }
  if (millis() - last_sensor_reading > averageReadingInterval) {
    last_sensor_reading = millis();
    outsideTemp->doReading();
    outsideHum->doReading();
    insideTemp->doReading();
    insideHum->doReading();
    dewPointOut->compute(outsideHum->getAverage(), outsideTemp->getAverage());
    dewPointIn->compute(insideHum->getAverage(), insideTemp->getAverage());
  }
  monitoring->act();
  button->act();
  ventilator->act();
  if (settingsData->recuperationOn) {
    recuperation->act();
    recuperationRelay->act();
  }
  ventilatorRelay->act();
  dns.processNextRequest();
  orchestrator->act();
  filter->act();
  rpmVentilatorChecker->actMaxTemp(outsideTemp->getAverage(), ventilator->getIntendedPower());
  if (settingsData->checkVentilatorRpm){
    if (rpmVentilatorChecker->act(ventilatorTicks, ventilator->getPower())) {
      detachVentilator();
      ventilatorTicks = 0;
      attachVentilator();  
    }
  }
  if (settingsData->checkRecuperationRpm && settingsData->recuperationOn){
    if (rpmRecuperationChecker->act(recuperationTicks, recuperation->getActualPower())) {
      detachRecuperation();
      recuperationTicks = 0;
      attachRecuperation();  
    }
  }
}
