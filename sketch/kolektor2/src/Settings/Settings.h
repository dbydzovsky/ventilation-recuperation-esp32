/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Settings_h
#define Settings_h
#include "Arduino.h"
#include "ArduinoJson.h"
#include "../Constants/Constants.h"
#include "../Debugger/Debugger.h"

// UNBLOCKING_FANS_PERIOD 172800000 // 2 days
// RECUPERATION_WAIT_FOR_DIRECTION_CHANGE 6000

// VENTILATOR_MAX_RPM 7600
// RECUPERATION_MAX_RPM 6000

struct SettingsData {
  bool recuperationOn = true;
  bool checkRecuperationRpm = true;
  bool checkVentilatorRpm = true;
  bool hideCo2 = false;
  bool hideInternalTempHum = false;
  int unblockingFansPeriod = 172800000;  // 2 days
  int ventilatorMaxRpm = 7600;
  int recuperationMaxRpm = 6000;
  int recuperationWaitForDirectionChange = 6000;
  int recuperationCycleDuration = 70000;
  int recuperationMode = RECUPERATION_MODE_RECYCLE;
  int recuperationPower = 100;
  int ventilatorPower = 0;
  int durationMillis = 300000;
  int tempDisableDuration = 10800000; // 3 hours
  int brightness = 1024;
  int maxVentilatorTemp = 70;
};

class Settings {
  public:
    Settings(Debugger * debugger);
    void setup();
    SettingsData* getSettings();
    bool save(JsonVariant &json);
  private:
    Debugger * debugger;
    SettingsData * data = {};
    bool validate(DynamicJsonDocument doc, SettingsData *out);
    bool loadJson(DynamicJsonDocument *doc);
};

#endif