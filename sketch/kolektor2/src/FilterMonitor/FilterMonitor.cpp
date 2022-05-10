#include "Arduino.h"
#include "ArduinoJson.h"
#include "FilterMonitor.h"
#include "../Recuperation/Recuperation.h"
#include "../Ventilator/Ventilator.h"
#include "../Constants/Constants.h"
#include "../Debugger/Debugger.h"
#include "../Settings/Settings.h"
#include "SPIFFS.h"

#define fanTrackingPersistenceInterval 3600000 // once per 1 hour
#define filterVentilatorDaysWithoutCleaning 60 // 2months
#define filterRecuperationDaysWithoutCleaning 90 // 3months

#define ventilator_filename "/ventilator.json"
#define recuperation_filename "/recuperation.json"


bool save(Debugger * debugger, const char* path, FilterData data) {
  char buff[90];
  sprintf(buff, "Going to save filter %s with minutes %d", path, data.minutes);
  debugger->trace(buff);
  StaticJsonDocument<256> jsonDoc;
  JsonObject root = jsonDoc.to<JsonObject>();
  root["minutes"] = data.minutes;
  File aFile = SPIFFS.open(path, "w");
  if (!aFile) {
    debugger->trace("ERR Cannot open file for writing");
    return false;
  }
  serializeJson(root, aFile);
  aFile.close();
}

void read(Debugger * debugger, const char* path, FilterData *out){ 
  char buff[50];
  sprintf(buff, "Going to load file %s", path);
  debugger->trace(buff);
  DynamicJsonDocument doc(256);
  File aFile = SPIFFS.open(path, "r");
  if (!aFile) {
    debugger->trace("ERR Cannot open file for reading");
    return;
  }
  deserializeJson(doc, aFile);
  aFile.close();
  out->minutes = doc["minutes"].as<long>();
  char buff2[50];
  sprintf(buff2, "File read, content minutes: %d", out->minutes);
  debugger->trace(buff2);
}

FilterMonitor::FilterMonitor(Ventilator * ventilator, Recuperation *recuperation, Debugger * debugger, Settings * settings){
  this->_recuperation = recuperation;
  this->_ventilator = ventilator;
  this->debugger = debugger;
  this->_settings = settings;
}

void FilterMonitor::setup() {
  FilterData ventilatorData;
  read(this->debugger, ventilator_filename, &ventilatorData);
  this->_ventilatorMinutes = ventilatorData.minutes;
  if (this->_settings->getSettings()->recuperationOn) {
    FilterData recuperationData;
    read(this->debugger ,recuperation_filename, &recuperationData);
    this->_recuperationMinutes = recuperationData.minutes;
  }
}

bool FilterMonitor::cleared(int filter){
  if (filter == FAN_TYPE_VENTILATOR) {
    this->debugger->trace("Going to clear ventilator");
    FilterData data;
    if (save(this->debugger, ventilator_filename, data)) {
      this->debugger->debug("Ventilator filter cleared.");
      this->_ventilatorMinutes = 0;
      this->_ventilatorChanged = false;
      return true;
    } else {
      this->debugger->debug("WARN Unable to clear Ventilator filter.");
    }
  }
  if (filter == FAN_TYPE_RECUPERATION && this->_settings->getSettings()->recuperationOn) {
    this->debugger->trace("Going to clear recuperation");
    FilterData data;
    if (save(this->debugger, recuperation_filename, data)) {
      this->debugger->debug("Recuperation filter cleared.");
      this->_recuperationMinutes = 0;
      this->_recuperationChanged = false;
      return true;
    } else {
      this->debugger->debug("WARN Unable to clear Ventilator filter.");
    }
  }
  this->debugger->debug("WARN Invalid type of filter to be cleaned");
  return false;
}

void FilterMonitor::report(byte filter, FilterReport * report) {
  if (filter == FAN_TYPE_VENTILATOR) {
    long liveMinutesTotal = filterVentilatorDaysWithoutCleaning * 24 * 60;
    report->remainMinutes = liveMinutesTotal - this->_ventilatorMinutes;
    report->needCleaning = report->remainMinutes <= 0;
  }
  if (filter == FAN_TYPE_RECUPERATION && this->_settings->getSettings()->recuperationOn) {
    long liveMinutesTotal = filterRecuperationDaysWithoutCleaning * 24 * 60;
    report->remainMinutes = liveMinutesTotal - this->_recuperationMinutes;
    report->needCleaning = report->remainMinutes <= 0;
  }
}

void FilterMonitor::persist() {
  FilterReport ventilatorReport;
  this->report(FAN_TYPE_VENTILATOR, &ventilatorReport);
  if (!ventilatorReport.needCleaning && this->_ventilatorChanged) {
    FilterData ventilatorData;
    ventilatorData.minutes = this->_ventilatorMinutes;
    save(this->debugger, ventilator_filename, ventilatorData);
    this->_ventilatorChanged = false;
  }
  if (this->_settings->getSettings()->recuperationOn) {
    FilterReport recupertionReport;
    this->report(FAN_TYPE_RECUPERATION, &recupertionReport);
    if (!recupertionReport.needCleaning && this->_recuperationChanged) {
      FilterData recuperationData;
      recuperationData.minutes = this->_recuperationMinutes;
      save(this->debugger, recuperation_filename, recuperationData);
      this->_recuperationChanged = false;
    }
  }
  this->_lastPersistence = millis();
}
void FilterMonitor::act(){ 
    if (millis() - this->_lastTracking > 60000) {
      this->_lastTracking = millis();
      short ventilatorPower = this->_ventilator->getPower();
      if (ventilatorPower != 0) {
        this->_ventilatorChanged = true;
        this->_ventilatorMinutes += 1;
      };
      if (this->_settings->getSettings()->recuperationOn) {
        if (this->_recuperation->getPower() != 0) {
          this->_recuperationChanged = true;
          this->_recuperationMinutes += 1;
        }
      }
      if (millis() - this->_lastPersistence > fanTrackingPersistenceInterval) {
        this->persist();
      }
    }
}
