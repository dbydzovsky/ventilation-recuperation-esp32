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
  out->hideCo2 = c["hideCo2"].as<bool>();
  out->hideInternalTempHum = c["hideInternalTempHum"].as<bool>();  
  out->unblockingFansPeriod = c["unblockingFansPeriod"].as<int>();
  if (out->unblockingFansPeriod < 1000) {
    if (IS_DEBUG) Serial.println("invalid unblockingFansPeriod");
    return false;
  }
  out->ventilatorMaxRpm = c["ventilatorMaxRpm"].as<int>();
  if (out->ventilatorMaxRpm < 1000) {
    if (IS_DEBUG) Serial.println("invalid ventilatorMaxRpm");
    return false;
  }
  out->recuperationMaxRpm = c["recuperationMaxRpm"].as<int>();
  if (out->recuperationMaxRpm < 1000) {
    if (IS_DEBUG) Serial.println("invalid recuperationMaxRpm");
    return false;
  }
  out->recuperationWaitForDirectionChange = c["recuperationWaitForDirectionChange"].as<int>();
  if (out->recuperationWaitForDirectionChange < 4000) {
    if (IS_DEBUG) Serial.println("invalid recuperationWaitForDirectionChange");
    return false;
  }
  out->recuperationCycleDuration = c["recuperationCycleDuration"].as<int>();
  if (out->recuperationCycleDuration < 40000) {
    if (IS_DEBUG) Serial.println("invalid recuperationCycleDuration");
    return false;
  }
  out->recuperationMode = c["recuperationMode"].as<int>();
  if (out->recuperationMode != RECUPERATION_MODE_RECYCLE &&
      out->recuperationMode != RECUPERATION_MODE_INHALE &&
      out->recuperationMode != RECUPERATION_MODE_EXHALE) {
    if (IS_DEBUG) Serial.println("invalid recuperationMode");
    return false;
  }
  out->recuperationPower = c["recuperationPower"].as<int>();
  if (out->recuperationPower < 0 || out->recuperationPower > 100) {
    if (IS_DEBUG) Serial.println("invalid recuperationPower");
    return false;
  }
  out->ventilatorPower = c["ventilatorPower"].as<int>();
  if (out->ventilatorPower < 0 || out->ventilatorPower > 100) {
    if (IS_DEBUG) Serial.println("invalid ventilatorPower");
    return false;
  }
  out->durationMillis = c["durationMillis"].as<int>();
  if (out->durationMillis < 60000) {
    if (IS_DEBUG) Serial.println("invalid durationMillis");
    return false;
  }
  out->tempDisableDuration = c["tempDisableDuration"].as<int>();
  if (out->tempDisableDuration < 5000) {
    return false;
  }
  out->brightness = c["brightness"].as<int>();
  if (out->brightness < 0 || out->brightness > 100) {
    return false;
  }
  return true;
}
