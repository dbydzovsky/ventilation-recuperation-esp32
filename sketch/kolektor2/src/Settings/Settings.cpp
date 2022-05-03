#include "Arduino.h"
#include "Settings.h"

#include "ArduinoJson.h"
#include <FS.h>
#include "SPIFFS.h"
#include "../Constants/Constants.h"

Settings::Settings(Debugger * debugger) {
  this->debugger = debugger;
}

void Settings::setup() {
  DynamicJsonDocument doc(4096);
  this->debugger->debug("Loading settings JSON.");
  if (this->loadJson(&doc)) {  
    SettingsData * data = new SettingsData();
    if (!this->validate(doc, data)) {
      this->debugger->debug("ERR Cannot load settings.json, because some of it value is not valid.");
      this->data = new SettingsData();
    } else {
      this->debugger->debug("Settings JSON validated.");
      this->data = data;
    }
  } else {
    this->debugger->debug("ERR Settings file cannot be loaded, using default.");
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
  out->recuperationOn = c["recuperationOn"].as<bool>();  
  out->unblockingFansPeriod = c["unblockingFansPeriod"].as<long>();
  if (out->unblockingFansPeriod < 1000) {
    this->debugger->debug("WARN Invalid unblockingFansPeriod, must not be less than 1000ms");
    return false;
  }
  out->ventilatorMaxRpm = c["ventilatorMaxRpm"].as<int>();
  if (out->ventilatorMaxRpm < 1000) {
    this->debugger->debug("WARN invalid ventilatorMaxRpm");
    return false;
  }
  out->recuperationMaxRpm = c["recuperationMaxRpm"].as<int>();
  if (out->recuperationMaxRpm < 1000) {
    this->debugger->debug("WARN invalid recuperationMaxRpm");
    return false;
  }
  out->recuperationWaitForDirectionChange = c["recuperationWaitForDirectionChange"].as<int>();
  if (out->recuperationWaitForDirectionChange < 4000) {
    this->debugger->debug("WARN invalid recuperationWaitForDirectionChange");
    return false;
  }
  out->recuperationCycleDuration = c["recuperationCycleDuration"].as<int>();
  if (out->recuperationCycleDuration < 40000) {
    this->debugger->debug("WARN invalid recuperationCycleDuration");
    return false;
  }
  out->recuperationMode = c["recuperationMode"].as<int>();
  if (out->recuperationMode != RECUPERATION_MODE_RECYCLE &&
      out->recuperationMode != RECUPERATION_MODE_INHALE &&
      out->recuperationMode != RECUPERATION_MODE_EXHALE) {
    this->debugger->debug("WARN invalid recuperationMode");
    return false;
  }
  out->recuperationPower = c["recuperationPower"].as<int>();
  if (out->recuperationPower < 0 || out->recuperationPower > 100) {
    this->debugger->debug("WARN invalid recuperationPower");
    return false;
  }
  out->ventilatorPower = c["ventilatorPower"].as<int>();
  if (out->ventilatorPower < 0 || out->ventilatorPower > 100) {
    this->debugger->debug("WARN invalid ventilatorPower");
    return false;
  }
  out->durationMillis = c["durationMillis"].as<int>();
  if (out->durationMillis < 60000) {
    this->debugger->debug("WARN invalid durationMillis");
    return false;
  }
  out->tempDisableDuration = c["tempDisableDuration"].as<int>();
  if (out->tempDisableDuration < 5000) {
    this->debugger->debug("WARN Invalid temp disable duration.");
    return false;
  }
  out->brightness = c["brightness"].as<int>();
  if (out->brightness < 0 || out->brightness > 100) {
    this->debugger->debug("WARN Invalid brightness");
    return false;
  }
  out->maxVentilatorTemp = c["maxVentilatorTemp"].as<int>();
  if (out->maxVentilatorTemp < 0 || out->maxVentilatorTemp > 80) {
    this->debugger->debug("WARN Invalid maxVentilatorTemp. Must be between 0 and 80.");
    return false;
  }
  out->ventilatorRevolutions = c["ventilatorRevolutions"].as<int>();
  if (out->ventilatorRevolutions < 1 || out->ventilatorRevolutions > 9) {
    this->debugger->debug("ventilatorRevolutions is invalid. Must be between 0 and 10.");
  }
  out->recuperationRevolutions = c["recuperationRevolutions"].as<int>();
  if (out->recuperationRevolutions < 1 || out->recuperationRevolutions > 9) {
    this->debugger->debug("recuperationRevolutions is invalid. Must be between 0 and 10.");
  }
  return true;
}
