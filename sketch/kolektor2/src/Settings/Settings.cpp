#include "Arduino.h"
#include "Settings.h"

#include "ArduinoJson.h"
#include <FS.h>
#include "SPIFFS.h"
#include "../Constants/Constants.h"



void Settings::setup() {
  if (IS_DEBUG) Serial.println("Goding to read settings..");
  DynamicJsonDocument doc(4096);
  if (IS_DEBUG) Serial.println("Loading json..");
  if (this->loadJson(&doc)) {
    if (IS_DEBUG) Serial.println("Settings json Loaded, validating..");
    SettingsData * data = new SettingsData();
    if (!this->validate(doc, data)) {
      Serial.println("Cannot load settings.json, because it is not valid.");
      this->data = new SettingsData();
    } else {
      if (IS_DEBUG) Serial.println("Json validated, saving as data..");
      this->data = data;
    }
  } else {
    if (IS_DEBUG) Serial.println("Settings file cannot be loaded, using default.");
    this->data = new SettingsData();
  }
}

SettingsData * Settings::getSettings() {
  return this->data;
}

bool Settings::loadJson(DynamicJsonDocument *doc) {
  File settingsFile = SPIFFS.open("/settings.json", "r");
  if (!settingsFile) {
    return false;
  }
  deserializeJson(*doc, settingsFile);
  settingsFile.close();
  return true;
}

bool Settings::save(JsonVariant &json) {
  SettingsData * newData = new SettingsData();
  if (!this->validate(json, newData)) {
    return false;
  }
  File settingsFile = SPIFFS.open("/settings.json", "w");
  if (!settingsFile) {
    return false;
  }
  serializeJson(json, settingsFile);
  settingsFile.close();
  return true;
}

bool Settings::validate(DynamicJsonDocument c, SettingsData *out) {
  out->checkRecuperationRpm = c["checkRecuperationRpm"].as<bool>();
  out->checkVentilatorRpm = c["checkVentilatorRpm"].as<bool>();
  out->unblockingFansPeriod = c["unblockingFansPeriod"].as<int>();
  out->ventilatorMaxRpm = c["ventilatorMaxRpm"].as<int>();
  out->recuperationMaxRpm = c["recuperationMaxRpm"].as<int>();
  out->syncForecastTolerateLastSuccessFor = c["syncForecastTolerateLastSuccessFor"].as<int>();
  out->syncForecastInterval = c["syncForecastInterval"].as<int>();
  out->relayCooldown = c["relayCooldown"].as<int>();
  if (out->relayCooldown < 2000 || out->relayCooldown > 15000) {
    return false;
  }
  out->recuperationWaitForDirectionChange = c["recuperationWaitForDirectionChange"].as<int>();
  if (out->recuperationWaitForDirectionChange < 4000) {
    return false;
  }
  out->recuperationCycleDuration = c["recuperationCycleDuration"].as<int>();
  if (out->recuperationCycleDuration < 30000) {
    return false;
  }
  return true;
}
