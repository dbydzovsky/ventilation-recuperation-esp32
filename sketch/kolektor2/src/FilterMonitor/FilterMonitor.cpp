#include "Arduino.h"
#include "ArduinoJson.h"
#include "FilterMonitor.h"
#include "../Recuperation/Recuperation.h"
#include "../Ventilator/Ventilator.h"
#include "../Constants/Constants.h"

#include "SPIFFS.h"

#define fanTrackingPersistenceInterval 21600000 // once per 6 hours
#define filterVentilatorDaysWithoutCleaning 60 // 2months
#define filterRecuperationDaysWithoutCleaning 90 // 3months

#define ventilator_filename "/ventilator.json"
#define recuperation_filename "/recuperation.json"


bool save(const char* path, FilterData data) {
  if (IS_DEBUG) Serial.print("Going to save file ");
  if (IS_DEBUG) Serial.println(path);
  StaticJsonDocument<256> jsonDoc;
  JsonObject root = jsonDoc.to<JsonObject>();
  root["minutes"] = data.minutes;
  File aFile = SPIFFS.open(path, "w");
  if (!aFile) {
    if (IS_DEBUG) Serial.println("Cannot open file for writing");
    return false;
  }
  serializeJson(root, aFile);
  aFile.close();
}

void read(const char* path, FilterData *out){ 
  if (IS_DEBUG) Serial.print("Going to load file ");
  if (IS_DEBUG) Serial.println(path);
  DynamicJsonDocument doc(256);
  File aFile = SPIFFS.open(path, "r");
  if (!aFile) {
    if (IS_DEBUG) Serial.println("Cannot open file for reading");
    return;
  }
  deserializeJson(doc, aFile);
  aFile.close();
  out->minutes = doc["minutes"].as<long>();
  if (IS_DEBUG) Serial.print("File read, content minutes: ");
  if (IS_DEBUG) Serial.println(out->minutes);
}

FilterMonitor::FilterMonitor(Ventilator * ventilator, Recuperation *recuperation){
  this->_recuperation = recuperation;
  this->_ventilator = ventilator;
}

void FilterMonitor::setup() {
  FilterData ventilatorData;
  read(ventilator_filename, &ventilatorData);
  this->_ventilatorMinutes = ventilatorData.minutes;
  
  FilterData recuperationData;
  read(recuperation_filename, &recuperationData);
  this->_recuperationMinutes = recuperationData.minutes;
}

bool FilterMonitor::cleared(int filter){
  if (filter == FILTER_MONITOR_VENTILATOR) {
    if (IS_DEBUG) Serial.println("Going to clear ventilator");
    FilterData data;
    if (save(ventilator_filename, data)) {
      if (IS_DEBUG) Serial.println("Ventilator cleared.");
      this->_ventilatorMinutes = 0;
      return true;
    }
  }
  if (filter == FILTER_MONITOR_RECUPERATION) {
    if (IS_DEBUG) Serial.println("Going to clear recuperation");
    FilterData data;
    if (save(recuperation_filename, data)) {
      if (IS_DEBUG) Serial.println("Recuperation cleared");
      this->_recuperationMinutes = 0;
      return true;
    }
  }
  return false;
}

void FilterMonitor::report(byte filter, FilterReport * report) {
  if (filter == FILTER_MONITOR_VENTILATOR) {
    long liveMinutesTotal = filterVentilatorDaysWithoutCleaning * 24 * 60;
    report->remainMinutes = liveMinutesTotal - this->_ventilatorMinutes;
    report->needCleaning = report->remainMinutes <= 0;
  }
  if (filter == FILTER_MONITOR_RECUPERATION) {
    long liveMinutesTotal = filterRecuperationDaysWithoutCleaning * 24 * 60;
    report->remainMinutes = liveMinutesTotal - this->_recuperationMinutes;
    report->needCleaning = report->remainMinutes <= 0;
  }
}

void FilterMonitor::act(){ 
    if (millis() - this->_lastTracking > 60000) {
      this->_lastTracking = millis();
      if (this->_ventilator->getPower() != 0) {
        if (IS_DEBUG) Serial.println("Adding a filter minute to ventilator");
        this->_ventilatorMinutes += 1;
      };
      if (this->_recuperation->getPower() != 0) {
        if (IS_DEBUG) Serial.println("Adding a filter minute to recuperation");
        this->_recuperationMinutes += 1;
      }
      if (millis() - this->_lastPersistence > fanTrackingPersistenceInterval) {
        FilterData ventilatorData;
        ventilatorData.minutes = this->_ventilatorMinutes;
        save(ventilator_filename, ventilatorData);
        FilterData recuperationData;
        recuperationData.minutes = this->_recuperationMinutes;
        save(recuperation_filename, recuperationData);
        this->_lastPersistence = millis();
      }
    }
}