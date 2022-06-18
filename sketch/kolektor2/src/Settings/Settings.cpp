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
      this->_isValid = true;
      this->debugger->debug("Settings JSON validated.");
      this->data = data;
    }
  } else {
    this->debugger->debug("ERR Settings file cannot be loaded, using default.");
    this->data = new SettingsData();
  }
  this->_permittedAll = false;
}

SettingsData * Settings::getSettings() {
  return this->data;
}
bool Settings::isValid() {
  return this->_isValid;
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

void Settings::permitAll(bool permission){ 
  this->_permittedAll = permission;
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
  this->_isValid = true;
  serializeJson(json, settingsFile);
  settingsFile.close();
  return true;
}

bool Settings::checkAccess(int a, int b) {
  if (this->_permittedAll) {
    return true;
  }
  return a == b;
}

bool Settings::validate(DynamicJsonDocument c, SettingsData *out) {
  out->checkRecuperationRpm = c["checkRecuperationRpm"].as<bool>();
  out->checkVentilatorRpm = c["checkVentilatorRpm"].as<bool>();
  out->testingMode = c["testingMode"].as<bool>();
  if (!this->checkAccess(out->testingMode, this->data->testingMode)) {
    this->debugger->debug("WARN testing mode cannot be changed due to missing perm.");
    return false;
  }
  out->hideCo2 = c["hideCo2"].as<bool>();
  out->hideInternalTempHum = c["hideInternalTempHum"].as<bool>();  
  out->recuperationOn = c["recuperationOn"].as<bool>();
  if (!this->checkAccess(out->recuperationOn, this->data->recuperationOn)) {
    this->debugger->debug("WARN cannot change recuperationOn due to missing permissions.");
    return false;
  }
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
  if (out->maxVentilatorTemp < 0 || out->maxVentilatorTemp > 70) {
    this->debugger->debug("WARN Invalid maxVentilatorTemp. Must be between 0 and 70.");
    return false;
  }
  out->ventilatorRevolutions = c["ventilatorRevolutions"].as<int>();
  if (out->ventilatorRevolutions < 1 || out->ventilatorRevolutions > 9) {
    this->debugger->debug("ventilatorRevolutions is invalid. Must be between 0 and 10.");
    return false;
  }
  if (!this->checkAccess(out->ventilatorRevolutions, this->data->ventilatorRevolutions)) {
    this->debugger->debug("WARN cannot change ventilatorRevolutions due to missing permissions.");
    return false;
  }
  out->recuperationRevolutions = c["recuperationRevolutions"].as<int>();
  if (out->recuperationRevolutions < 1 || out->recuperationRevolutions > 9) {
    this->debugger->debug("recuperationRevolutions is invalid. Must be between 0 and 10.");
    return false;
  }
  if (!this->checkAccess(out->recuperationRevolutions, this->data->recuperationRevolutions)) {
    this->debugger->debug("WARN cannot change recuperationRevolutions due to missing permissions.");
    return false;
  }
  out->recuperationMhz = c["recuperationMhz"].as<int>();
  if (out->recuperationMhz != 20000 && out->recuperationMhz != 25000) {
    char buff[90];
    sprintf(buff, "recuperationMhz is invalid. Must be 20khz or 25khz, but is %d", out->recuperationMhz);
    this->debugger->debug(buff);
    return false;
  }
  if (!this->checkAccess(out->recuperationMhz, this->data->recuperationMhz)) {
    this->debugger->debug("WARN cannot change recuperationMhz due to missing permissions.");
    return false;
  }
  out->ventilationMhz = c["ventilationMhz"].as<int>();
  if (out->ventilationMhz != 20000 && out->ventilationMhz != 25000) {
    char buff[90];
    sprintf(buff, "ventilationMhz is invalid. Must be 20khz or 25khz, but is %d", out->ventilationMhz);
    this->debugger->debug(buff);
    return false;
  }
  if (!this->checkAccess(out->ventilationMhz, this->data->ventilationMhz)) {
    this->debugger->debug("WARN cannot change ventilationMhz due to missing permissions.");
    return false;
  }
  out->ventilationRelayPin = c["ventilationRelayPin"].as<int>();
  if (out->ventilationRelayPin != 17 && out->ventilationRelayPin != 19) {
    char buff[90];
    sprintf(buff, "ventilationRelayPin is invalid. Must be 17 or 19, but is %d", out->ventilationRelayPin);
    this->debugger->debug(buff);
    return false;
  }
  if (!this->checkAccess(out->ventilationRelayPin, this->data->ventilationRelayPin)) {
    this->debugger->debug("WARN cannot change ventilationRelayPin due to missing permissions.");
    return false;
  }
  out->recuperationRelayPin = c["recuperationRelayPin"].as<int>();
  if (out->recuperationRelayPin != 17 && out->recuperationRelayPin != 19) {
    char buff[90];
    sprintf(buff, "recuperationRelayPin is invalid. Must be 17 or 19, but is %d", out->recuperationRelayPin);
    this->debugger->debug(buff);
    return false;
  }
  if (!this->checkAccess(out->recuperationRelayPin, this->data->recuperationRelayPin)) {
    this->debugger->debug("WARN cannot change ventilationRelayPin due to missing permissions.");
    return false;
  }
  return true;
}
