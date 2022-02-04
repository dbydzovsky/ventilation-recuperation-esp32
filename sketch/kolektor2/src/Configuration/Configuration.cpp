#include "Arduino.h"
#include "Configuration.h"

#include "ArduinoJson.h"
#include <FS.h>
#include "SPIFFS.h"

void Configuration::setInactiveMode() {
  this->changeProperty("mo", INACTIVE_MODE);
}

bool Configuration::changeProperty(const char* name, short value) {
  DynamicJsonDocument doc(1024);
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
  DynamicJsonDocument doc(1024);
  if (this->loadJson(&doc)) {
    ConfigurationData * data = new ConfigurationData();
    if (!this->validate(doc, data)) {
      Serial.println("Cannot load config.json");
      return;
    }
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
  const char* name = c["n"].as<const char*>();
  if (strlen(name) > 29) {
    return false;
  }
  strncpy(out->name, name, 30);
  out->mode = c["mo"].as<byte>();
  out->autoWinterStart = c["aws"].as<short>();
  if (out->autoWinterStart < 0 || out->autoWinterStart > 365) {
    return false;
  }
  out->autoWinterEnd = c["awe"].as<short>();
  if (out->autoWinterEnd < 0 || out->autoWinterEnd > 365) {
    return false;
  }
  out->autoSummerStart = c["ass"].as<short>();
  if (out->autoSummerStart < 0 || out->autoSummerStart > 365) {
    return false;
  }
  out->autoSummerEnd = c["ase"].as<short>();
  if (out->autoSummerEnd < 0 || out->autoSummerEnd > 365) {
    return false;
  }
  if (this->isOverlapping(out->autoSummerStart,out->autoSummerEnd,out->autoWinterStart,out->autoWinterEnd)) {
    return false;
  }
  
  out->winterMaxInsideTemp = c["wmit"].as<short>();
  if (out->winterMaxInsideTemp < 0 || out->winterMaxInsideTemp > 1000) {
    return false;
  }

  out->summerMinInsideTemp = c["smit"].as<short>();
  if (out->summerMinInsideTemp < 0 || out->summerMinInsideTemp > 1000) {
    return false;
  }
  Monitoring * monitoring = new Monitoring();
  out->monitoring = monitoring;
  const char* apikey = c["m"]["k"].as<const char*>();
  if (strlen(apikey) > 49) {
    return false;
  }
  strncpy(monitoring->apikey, apikey, 50);
  const char* feed = c["m"]["f"].as<const char*>();
  if (strlen(feed) > 29) {
    return false;
  }
  strncpy(monitoring->feed, feed, 30);
  int winterOnRulesSize = c["wor"].size();
  if (winterOnRulesSize > 5) {
    return false;
  }

  const char* weatherApiKey = c["w"].as<const char*>();
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

  Rules * onRules = new Rules();
  onRules->count = byte(winterOnRulesSize);
  out->winterOnRules = onRules;
  for (short i = 0; i < winterOnRulesSize; i++) {
    Rule * rule = new Rule();
    rule->temperature = c["wor"][i]["t"].as<byte>();
    rule->percentage = c["wor"][i]["p"].as<byte>();
    if (rule->percentage < 0 || rule->percentage > 100) {
      return false;
    }
    if (rule->temperature < 0 || rule->temperature > 100) {
      return false;
    }
    onRules->rules[i] = rule;
    if (i != 0) {
      Rule * previous = onRules->rules[i - 1];
      if (previous->temperature > (rule->temperature - 3)) {
        return false;
      }
    }
  }
  int summerOnRulesSize = c["sor"].size();
  if (summerOnRulesSize > 5) {
    return false;
  }
  Rules * summerOnRules = new Rules();
  summerOnRules->count = byte(summerOnRulesSize);
  out->summerOnRules = summerOnRules;
  for (short i = 0; i < summerOnRulesSize; i++) {
    Rule * rule = new Rule();
    rule->temperature = c["sor"][i]["t"].as<byte>();
    rule->percentage = c["sor"][i]["p"].as<byte>();
    if (rule->percentage < 0 || rule->percentage > 100) {
      return false;
    }
    if (rule->temperature < 0 || rule->temperature > 100) {
      return false;
    }
    summerOnRules->rules[i] = rule;
    if (i != 0) {
      Rule * previous = summerOnRules->rules[i - 1];
      if (previous->temperature < (rule->temperature + 3)) {
        return false;
      }
    }
  }
  return true;
}
