#include "Arduino.h"
#include "Configuration.h"

#include "ArduinoJson.h"
#include <FS.h>
#include "SPIFFS.h"
#include "../Constants/Constants.h"

void Configuration::setInactiveMode() {
  this->changeProperty("mo", INACTIVE_MODE);
}

bool Configuration::changeProperty(const char* name, short value) {
  if (IS_DEBUG) Serial.println("Changing property.");
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
  if (IS_DEBUG) Serial.println("Goding to allocate doc..");
  DynamicJsonDocument doc(4096);
  if (IS_DEBUG) Serial.println("Loading json..");
  if (this->loadJson(&doc)) {
    if (IS_DEBUG) Serial.println("Json Loaded, validating..");
    ConfigurationData * data = new ConfigurationData();
    if (!this->validate(doc, data)) {
      Serial.println("Cannot load config.json");
      return;
    }
    if (IS_DEBUG) Serial.println("Json validated, saving..");
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
    delete oldData->monitoring;
    delete oldData;
    interrupts();
  }
  return true;
}

bool Configuration::validate(DynamicJsonDocument c, ConfigurationData *out) {
  if (IS_DEBUG) Serial.println("Parsing name");
  const char* name = c["name"].as<const char*>();
  if (strlen(name) > 29) {
    return false;
  }
  strncpy(out->name, name, 30);
  if (IS_DEBUG) Serial.println("Parsing mode");
  out->mode = c["mode"].as<byte>();
  out->autoWinterStart = c["autoWinterStart"].as<short>();
  if (out->autoWinterStart < 0 || out->autoWinterStart > 365) {
    return false;
  }
  out->autoWinterEnd = c["autoWinterEnd"].as<short>();
  if (out->autoWinterEnd < 0 || out->autoWinterEnd > 365) {
    return false;
  }
  out->autoSummerStart = c["autoSummerStart"].as<short>();
  if (out->autoSummerStart < 0 || out->autoSummerStart > 365) {
    return false;
  }
  out->autoSummerEnd = c["autoSummerEnd"].as<short>();
  if (out->autoSummerEnd < 0 || out->autoSummerEnd > 365) {
    return false;
  }
  if (this->isOverlapping(out->autoSummerStart,out->autoSummerEnd,out->autoWinterStart,out->autoWinterEnd)) {
    return false;
  }
  
  out->winterMaxInsideTemp = c["winterMaxInsideTemp"].as<float>();
  if (out->winterMaxInsideTemp < 0 || out->winterMaxInsideTemp > 1000) {
    return false;
  }

  out->summerMinInsideTemp = c["summerMinInsideTemp"].as<float>();
  if (out->summerMinInsideTemp < 0 || out->summerMinInsideTemp > 1000) {
    return false;
  }
  if (IS_DEBUG) Serial.println("Parsing monitoring");
  MonitoringData * monitoring = new MonitoringData();
  out->monitoring = monitoring;
  const char* apikey = c["monitoring"]["key"].as<const char*>();
  if (strlen(apikey) > 49) {
    return false;
  }
  strncpy(monitoring->apikey, apikey, 50);
  const char* feed = c["monitoring"]["feed"].as<const char*>();
  if (strlen(feed) > 29) {
    return false;
  }
  strncpy(monitoring->feed, feed, 30);
  int winterOnRulesSize = c["winterOnRules"].size();
  if (winterOnRulesSize > 5) {
    return false;
  }
  if (IS_DEBUG) Serial.println("Parsing weather key");
  const char* weatherApiKey = c["weatherApiKey"].as<const char*>();
  if (strlen(weatherApiKey) > 33) {
    return false;
  }
  strncpy(out->weatherApiKey, weatherApiKey, 33);

  const char* lat = c["lat"].as<const char*>();
  if (strlen(lat) > 5) {
    return false;
  }
  strncpy(out->lat, lat, 6);

  const char* lon = c["lon"].as<const char*>();
  if (strlen(lon) > 5) {
    return false;
  }
  strncpy(out->lon, lon, 6);
  if (IS_DEBUG) Serial.println("Parsing winter rules.");
  Rules * onRules = new Rules();
  onRules->count = byte(winterOnRulesSize);
  out->winterOnRules = onRules;
  for (short i = 0; i < winterOnRulesSize; i++) {
    Rule * rule = new Rule();
    rule->targetValue = c["winterOnRules"][i]["tv"].as<int>();
    rule->percentage = c["winterOnRules"][i]["p"].as<byte>();
    if (rule->percentage < 0 || rule->percentage > 100) {
      return false;
    }
    if (rule->targetValue < 0 || rule->targetValue > 100) {
      return false;
    }
    onRules->rules[i] = rule;
    if (i != 0) {
      Rule * previous = onRules->rules[i - 1];
      if (previous->targetValue > (rule->targetValue - 3)) {
        return false;
      }
    }
  }
  if (IS_DEBUG) Serial.println("Parsing summer rules");
  int summerOnRulesSize = c["summerOnRules"].size();
  if (summerOnRulesSize > 5) {
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
      return false;
    }
    if (rule->targetValue < 0 || rule->targetValue > 100) {
      return false;
    }
    summerOnRules->rules[i] = rule;
    if (i != 0) {
      Rule * previous = summerOnRules->rules[i - 1];
      if (previous->targetValue < (rule->targetValue + 3)) {
        return false;
      }
    }
  }
  if (IS_DEBUG) Serial.println("Parsing co2 rules.");
  int co2RulesSize = c["co2Rules"].size();
  if (co2RulesSize > 5) {
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
      return false;
    }
    if (rule->targetValue < 0 || rule->targetValue > 5000) {
      return false;
    }
    co2Rules->rules[i] = rule;
    if (i != 0) {
      Rule * previous = co2Rules->rules[i - 1];
      if (previous->targetValue > (rule->targetValue - 50)) {
        return false;
      }
    }
  }
  return true;
}
