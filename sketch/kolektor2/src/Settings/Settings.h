/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Settings_h
#define Settings_h
#include "Arduino.h"
#include "ArduinoJson.h"

// UNBLOCKING_FANS_PERIOD 172800000 // 2 days
// syncForecastTolerateLastSuccessFor 172800000 // 2 days
// syncForecastInterval 43200000 // 12 hours
// RECUPERATION_WAIT_FOR_DIRECTION_CHANGE 6000

// VENTILATOR_MAX_RPM 7600
// RECUPERATION_MAX_RPM 6000

struct SettingsData {
  bool checkRecuperationRpm = true;
  bool checkVentilatorRpm = true;
  int unblockingFansPeriod = 172800000;  // 2 days
  int ventilatorMaxRpm = 7600;
  int recuperationMaxRpm = 6000;
  int syncForecastTolerateLastSuccessFor = 172800000;  // 2 days
  int syncForecastInterval = 43200000; // 12 hours
  int relayCooldown = 5000;
  int recuperationWaitForDirectionChange = 6000;
  int recuperationCycleDuration = 70000;
};

class Settings {
  public:
    void setup();
    SettingsData* getSettings();
    bool save(JsonVariant &json);
  private:
    SettingsData * data = {};
    bool validate(DynamicJsonDocument doc, SettingsData *out);
    bool loadJson(DynamicJsonDocument *doc);
};

#endif