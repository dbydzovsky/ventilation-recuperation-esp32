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
  bool recuperationOn = false;
  bool checkRecuperationRpm = true;
  bool checkVentilatorRpm = true;
  bool hideCo2 = true;
  bool hideInternalTempHum = true;
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
  int ventilatorRevolutions = 2;
  int recuperationRevolutions = 2;
  int ventilationMhz = 25000;
  int recuperationMhz = 25000;
  int ventilationRelayPin = 17;
  int recuperationRelayPin = 19;
};

class Settings {
  public:
    Settings(Debugger * debugger);
    void setup();
    SettingsData* getSettings();
    bool save(JsonVariant &json);
    bool isValid();
    void permitAll(bool permission);
  private:
    bool _permittedAll = true; 
    bool _isValid = false;
    Debugger * debugger;
    SettingsData * data = new SettingsData();
    bool checkAccess(int a, int b);
    bool validate(DynamicJsonDocument doc, SettingsData *out);
    bool loadJson(DynamicJsonDocument *doc);
};

#endif