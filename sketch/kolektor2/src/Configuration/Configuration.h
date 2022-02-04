/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Configuration_h
#define Configuration_h
#include "Arduino.h"
#include "ArduinoJson.h"

#define WINTER_MODE 1
#define SUMMER_MODE 2
#define INACTIVE_MODE 0
#define AUTO_MODE 3


class Rule {
  public:
    byte temperature = 0;
    byte percentage = 0;
};

class Rules {
  public:
    Rule* rules[5];
    byte count = 0;
};

struct Monitoring {
  char feed[30] = "";
  char apikey[50] = "";
};

struct ConfigurationData {
  char name[30] = "";
  byte mode = INACTIVE_MODE;
  short winterMaxInsideTemp = 0;
  short summerMinInsideTemp = 0;
  Rules *winterOnRules;
  Rules *summerOnRules;
  Monitoring *monitoring;
  short autoWinterStart = 0;
  short autoWinterEnd = 0;
  short autoSummerStart = 0;
  short autoSummerEnd = 0;
  char weatherApiKey[33] = "";
  char lat[6] = "";
  char lon[6] = "";
};

class Configuration {
  public:
    void setup();
    void setInactiveMode();
    ConfigurationData* getData();
    bool save(JsonVariant &json);
    bool dataSet = false;
  private:
    ConfigurationData * data = {};
    bool isOverlapping(short a, short b, short c, short d);
    bool changeProperty(const char* name, short value);
    bool validate(DynamicJsonDocument doc, ConfigurationData *out);
    bool saveJson(DynamicJsonDocument doc);
    bool loadJson(DynamicJsonDocument *doc);
};

#endif