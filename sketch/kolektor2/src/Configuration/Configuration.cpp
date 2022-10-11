#include "Arduino.h"
#include "Configuration.h"

#include "ArduinoJson.h"
#include <FS.h>
#include "SPIFFS.h"
#include "../Constants/Constants.h"
#include "../Debugger/Debugger.h"

Configuration::Configuration(Debugger * debugger) {
  this->debugger = debugger;
}
bool Configuration::isValid() {
  return this->_isValid;
}
void Configuration::setInactiveMode() {
  this->changeProperty("mo", INACTIVE_MODE);
}

bool Configuration::changeProperty(const char* name, short value) {
  DynamicJsonDocument doc(4096);
  if (this->loadJson(&doc)) {
    doc[name] = value;
    return this->saveJson(doc);
  }
  return false;
}

bool Configuration::save(JsonVariant &json) {
  return this->saveJson(json);
}

bool Configuration::isOverlapping(short a, short b, short c, short d) {
    if (a < b && c < d) {
        return a < d && c < b;
    }
    if (a > b && c > d) {
        return true;
    }
    short m = 365;
    short w0 = (b - a) % m;
    short w1 = (d - c) % m;
    return (w1 != 0 && ((c - a)%m) < w0) || (w0 != 0 && ((a - c)%m) < w1);
}

void Configuration::setup() {
  this->debugger->debug("Allocating doc for config.json..");
  DynamicJsonDocument doc(4096);
  this->debugger->debug("Loading config.json.");
  if (this->loadJson(&doc)) {
    this->debugger->debug("Json Loaded, validating..");
    ConfigurationData * data = new ConfigurationData();
    if (!this->validate(doc, data)) {
      this->debugger->debug("ERR Cannot load config.json");
      return;
    }
    this->_isValid = true;
    this->debugger->debug("Json Validated, saving..");
    this->data = data;
    this->dataSet = true;
  }
}

ConfigurationData* Configuration::getData() {
  return this->data;
}

bool Configuration::loadJson(DynamicJsonDocument *doc) {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    return false;
  }
  deserializeJson(*doc, configFile);
  configFile.close();
  return true;
}

bool Configuration::saveJson(DynamicJsonDocument doc) {
  ConfigurationData * newData = new ConfigurationData();
  if (!this->validate(doc, newData)) {
    return false;
  }
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    return false;
  }
  serializeJson(doc, configFile);
  configFile.close();
  ConfigurationData * oldData = this->data;
  bool hasBeenSet = this->dataSet;
  this->data = newData;
  this->dataSet = true;
  if (hasBeenSet) {
    noInterrupts();
    for (int i = 0; i < oldData->winterOnRules->count; i++) {
      delete oldData->winterOnRules->rules[i];
    }
    delete oldData->winterOnRules;
    for (int i = 0; i < oldData->summerOnRules->count; i++) {
      delete oldData->summerOnRules->rules[i];
    }
    delete oldData->summerOnRules;
    for (int i = 0; i < oldData->co2Rules->count; i++) {
      delete oldData->co2Rules->rules[i];
    }
    delete oldData->co2Rules;
    delete oldData->monitoring;
    delete oldData;
    interrupts();
  }
  this->_isValid = true;
  return true;
}

bool Configuration::validate(DynamicJsonDocument c, ConfigurationData *out) {
  const char* name = c["name"].as<const char*>();
  if (strlen(name) > 29) {
    this->debugger->debug("WARN Name property is too long");
    return false;
  }
  if (c.containsKey("silentModeTill")) {
    out->silentModeTill = c["silentModeTill"].as<short>();
    if (out->silentModeTill < 0 || out->silentModeTill >= 1440) {
      this->debugger->debug("WARN silentModeTill must be between 0 and 1440.");
      return false;
    }
  } else {
    this->debugger->debug("silentModeTill setting to default value 1350.");
    out->silentModeTill = 1350;
  }
  strncpy(out->name, name, 30);
  out->mode = c["mode"].as<byte>();
  out->autoWinterStart = c["autoWinterStart"].as<short>();
  if (out->autoWinterStart < 0 || out->autoWinterStart > 365) {
    this->debugger->debug("WARN Winter start is out of range");
    return false;
  }
  out->autoWinterEnd = c["autoWinterEnd"].as<short>();
  if (out->autoWinterEnd < 0 || out->autoWinterEnd > 365) {
    this->debugger->debug("WARN Winter end is out of range");
    return false;
  }
  out->autoSummerStart = c["autoSummerStart"].as<short>();
  if (out->autoSummerStart < 0 || out->autoSummerStart > 365) {
    this->debugger->debug("WARN Summer start is out of range");
    return false;
  }
  out->autoSummerEnd = c["autoSummerEnd"].as<short>();
  if (out->autoSummerEnd < 0 || out->autoSummerEnd > 365) {
    this->debugger->debug("WARN Summer end is out of range");
    return false;
  }
  if (this->isOverlapping(out->autoSummerStart,out->autoSummerEnd,out->autoWinterStart,out->autoWinterEnd)) {
    this->debugger->debug("WARN Summer overlaps with Winter.");
    return false;
  }
  
  out->winterMaxInsideTemp = c["winterMaxInsideTemp"].as<float>();
  if (out->winterMaxInsideTemp < 0 || out->winterMaxInsideTemp > 1000) {
    this->debugger->debug("WARN Winter max inside must be between 0 and 1000.");
    return false;
  }

  out->summerMinInsideTemp = c["summerMinInsideTemp"].as<float>();
  if (out->summerMinInsideTemp < 0 || out->summerMinInsideTemp > 1000) {
    this->debugger->debug("WARN Summer minimum inside must be between 0 and 1000.");
    return false;
  }
  out->minimumFeelsLike = c["minimumFeelsLike"].as<float>();
  if (out->minimumFeelsLike < 0 || out->minimumFeelsLike > 1000) {
    this->debugger->debug("WARN Minimum feels like must be between 0 and 1000.");
    return false;
  }
  this->debugger->trace("Parsing monitoring");
  MonitoringData * monitoring = new MonitoringData();
  out->monitoring = monitoring;
  const char* apikey = c["monitoring"]["key"].as<const char*>();
  if (strlen(apikey) > 49) {
    this->debugger->debug("WARN Api key for monitoring must be 49 characters and less.");
    return false;
  }
  strncpy(monitoring->apikey, apikey, 50);
  const char* feed = c["monitoring"]["feed"].as<const char*>();
  if (strlen(feed) > 29) {
    this->debugger->debug("WARN Feed number must be 29 characters and less.");
    return false;
  }
  strncpy(monitoring->feed, feed, 30);
  int winterOnRulesSize = c["winterOnRules"].size();
  if (winterOnRulesSize > 5) {
    this->debugger->debug("WARN Max 5 winter rules are allowed.");
    return false;
  }
  this->debugger->trace("Parsing weather key");
  const char* weatherApiKey = c["weatherApiKey"].as<const char*>();
  if (strlen(weatherApiKey) > 33) {
    this->debugger->debug("WARN Weather api key must be 33 chars and less.");
    return false;
  }
  strncpy(out->weatherApiKey, weatherApiKey, 33);

  const char* lat = c["lat"].as<const char*>();
  if (strlen(lat) > 5) {
    this->debugger->debug("WARN Latitude must be 5 chars and less.");
    return false;
  }
  strncpy(out->lat, lat, 6);

  const char* lon = c["lon"].as<const char*>();
  if (strlen(lon) > 5) {
    this->debugger->debug("WARN Longitude must be 5 chars and less.");
    return false;
  }
  strncpy(out->lon, lon, 6);
  this->debugger->trace("Parsing winter rules.");
  Rules * onRules = new Rules();
  onRules->count = byte(winterOnRulesSize);
  out->winterOnRules = onRules;
  for (short i = 0; i < winterOnRulesSize; i++) {
    Rule * rule = new Rule();
    rule->targetValue = c["winterOnRules"][i]["tv"].as<int>();
    rule->percentage = c["winterOnRules"][i]["p"].as<byte>();
    if (rule->percentage < 0 || rule->percentage > 100) {
      this->debugger->debug("WARN Each winter rule must have percentage from 0 to 100");
      return false;
    }
    if (rule->targetValue < 0 || rule->targetValue > 100) {
      this->debugger->debug("WARN Each winter rule must have temperate from 0 to 100");
      return false;
    }
    onRules->rules[i] = rule;
    if (i != 0) {
      Rule * previous = onRules->rules[i - 1];
      if (previous->targetValue > (rule->targetValue - 3)) {
        this->debugger->debug("WARN Each winter rule must have 3 points gap after another one");
        return false;
      }
    }
  }
  this->debugger->trace("Parsing summer rules");
  int summerOnRulesSize = c["summerOnRules"].size();
  if (summerOnRulesSize > 5) {
    this->debugger->debug("WARN Max 5 summer rules are allowed.");
    return false;
  }
  Rules * summerOnRules = new Rules();
  summerOnRules->count = byte(summerOnRulesSize);
  out->summerOnRules = summerOnRules;
  for (short i = 0; i < summerOnRulesSize; i++) {
    Rule * rule = new Rule();
    rule->targetValue = c["summerOnRules"][i]["tv"].as<int>();
    rule->percentage = c["summerOnRules"][i]["p"].as<byte>();
    if (rule->percentage < 0 || rule->percentage > 100) {
      this->debugger->debug("WARN Each winter rule must have percentage from 0 to 100");
      return false;
    }
    if (rule->targetValue < 0 || rule->targetValue > 100) {
      this->debugger->debug("WARN Each winter rule must have temperature from 0 to 100");
      return false;
    }
    summerOnRules->rules[i] = rule;
    if (i != 0) {
      Rule * previous = summerOnRules->rules[i - 1];
      if (previous->targetValue < (rule->targetValue + 3)) {
        this->debugger->debug("WARN Each summer rule must have 3 points gap after another one");
        return false;
      }
    }
  }
  this->debugger->trace("Parsing co2 rules.");
  int co2RulesSize = c["co2Rules"].size();
  if (co2RulesSize > 5) {
    this->debugger->debug("WARN Max 5 co2 rules are allowed.");
    return false;
  }
  Rules * co2Rules = new Rules();
  co2Rules->count = byte(co2RulesSize);
  out->co2Rules = co2Rules;
  for (short i = 0; i < co2RulesSize; i++) {
    Rule * rule = new Rule();
    rule->targetValue = c["co2Rules"][i]["tv"].as<int>();
    rule->percentage = c["co2Rules"][i]["p"].as<short>();
    if (rule->percentage < 0 || rule->percentage > 100) {
      this->debugger->debug("WARN Each co2 rule must have percentage from 0 to 100");
      return false;
    }
    if (rule->targetValue < 0 || rule->targetValue > 5000) { 
      this->debugger->debug("WARN Each co2 rule must have value between 0 to 5000");
      return false;
    }
    co2Rules->rules[i] = rule;
    if (i != 0) {
      Rule * previous = co2Rules->rules[i - 1];
      if (previous->targetValue > (rule->targetValue - 50)) {
        this->debugger->debug("WARN Each co2 rule must have 50 points gap after another one");
        return false;
      }
    }
  }
  return true;
}
