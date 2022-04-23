#include "Arduino.h"
#include "ArduinoJson.h"
#include "RPMChecker.h"
#include "../Constants/Constants.h"
#include "SPIFFS.h"

#include "../Debugger/Debugger.h"
struct AlarmData {
  bool blocked = false;
  bool highRpm = false;
};

bool save(const char* path, AlarmData data) {
  if (IS_DEBUG) Serial.print("Going to save file ");
  if (IS_DEBUG) Serial.println(path);
  StaticJsonDocument<256> jsonDoc;
  JsonObject root = jsonDoc.to<JsonObject>();
  root["blocked"] = data.blocked;
  root["highRpm"] = data.highRpm;
  File aFile = SPIFFS.open(path, "w");
  if (!aFile) {
    if (IS_DEBUG) Serial.println("Cannot open file for writing");
    return false;
  }
  serializeJson(root, aFile);
  aFile.close();
}

void read(const char* path, AlarmData *out){ 
  if (IS_DEBUG) Serial.print("Going to load file ");
  if (IS_DEBUG) Serial.println(path);
  DynamicJsonDocument doc(256);
  File aFile = SPIFFS.open(path, "r");
  if (!aFile) {
    if (IS_DEBUG) Serial.println("Cannot open file for reading");
    return;
  }
  deserializeJson(doc, aFile);
  aFile.close();
  out->blocked = doc["blocked"].as<bool>();
  out->highRpm = doc["highRpm"].as<bool>();
  if (IS_DEBUG) Serial.print("File read, blocked: ");
  if (IS_DEBUG) Serial.println(out->blocked);
  if (IS_DEBUG) Serial.print("high rpm: ");
  if (IS_DEBUG) Serial.println(out->highRpm);
}

RPMChecker::RPMChecker(int pin, const char* filename, Debugger * debugger){
  this->_pin = pin;
  this->_filename = filename;
  this->debugger = debugger;
}

void RPMChecker::deactivate() {
  this->_activated = false;
}

void RPMChecker::setup() {
  pinMode(this->_pin, INPUT);
  AlarmData data;
  read(this->_filename, &data);
  if (data.blocked || data.highRpm) {
    this->_stopped = true;
    if (data.blocked) {
      char messageBuf[50];
      sprintf(messageBuf, "Motor %s is blocked since start", this->_filename);
      this->debugger->debug(messageBuf);
      this->_reason = MOTOR_BLOCKED_REASON;
    }
    if (data.highRpm) {
      char messageBuf[50];
      sprintf(messageBuf, "Motor %s had unexpected rotation", this->_filename);
      this->debugger->debug(messageBuf);
      this->_reason = MOTOR_HIGH_RPM_REASON;
    }
    this->_stoppedSince = millis();
  }
}
// todo kdyz co2 cidlo neni pripojene
float computeRpm(short ticksPerRevolution, int durationMs, long ticks) {
  int revolutions = ticks / ticksPerRevolution;
  float rpm = (60000/durationMs) * revolutions;
  return rpm;
}

void RPMChecker::setTicksPerRevolution(short ticksPerRevolution) {
  this->_ticksPerRevolution = ticksPerRevolution;
}

int RPMChecker::getRpm() {
  return this->_rpm;
}
void RPMChecker::setUnblockingFansPeriod(int unblockingPeriodSeconds) {
  this->_unblockingFansPeriod = unblockingPeriodSeconds;
}
void RPMChecker::setMaxRpm(int maxRpm) {
  this->_maxRpm = maxRpm;
}
void RPMChecker::setMaxTemperature(int maxTemp) {
  this->_checkMaxTemp = true;
  this->_maxTemp = maxTemp;
}
bool RPMChecker::actMaxTemp(int actualTemp, short currentPower) {
  if (isnan(actualTemp)) {
    return true;
  }
  bool overTempAlarm = false;
  if (this->_checkMaxTemp) {
    if (actualTemp > this->_maxTemp && currentPower > 0) {
      if (this->_overtempInitialized) {
        overTempAlarm = millis() - this->_overTempSince > MAX_OVERTEMPERATURE_PERIOD;
      } else {
        this->_overTempSince = millis();
        this->_overtempInitialized = true;
      }
    } else {
      this->_overtempInitialized = false;
    }
  }
  if (overTempAlarm && this->_overheated != overTempAlarm) {
    char messageBuf[100];
    sprintf(messageBuf, "WARN Ventilator motor stopped due to critical TEMPERATURE %d C", actualTemp);
    this->debugger->debug(messageBuf);
  }
  this->_overheated = overTempAlarm;
}
bool RPMChecker::act(long ticks, short currentPower) {
  // return true if counter should be reset
  if (currentPower != this->_lastPower) {
    this->last_sample = millis();
    this->_lastPower = currentPower;
    return true;
  }
  if (this->_stopped) {
    if (millis() - this->_stoppedSince > this->_unblockingFansPeriod) {
      AlarmData data;
      save(this->_filename, data);
      this->_stopped = false;
    }
    return true;
  }
  if (this->_checkMaxTemp && this->_overheated) {
    return true;
  }
  int duration = millis() - this->last_sample;
  if (duration > RPM_SAMPLING_DURATION) {
    this->last_sample = millis();
    this->_rpm = computeRpm(this->_ticksPerRevolution, duration, ticks);
    if (this->_rpm > this->_maxRpm) {
      AlarmData data;
      data.highRpm = true;
      if (save(this->_filename, data)){
        char messageBuf[50];
        sprintf(messageBuf, "WARN Motor %s has high rotation!", this->_filename);
        this->debugger->debug(messageBuf);
      } else {
        char messageBuf[50];
        sprintf(messageBuf, "ERR Motor %s has high rotation and unable to save!", this->_filename);
        this->debugger->debug(messageBuf);
      }
      this->_stopped = true;
      this->_stoppedSince = millis();
      this->_reason = MOTOR_HIGH_RPM_REASON;
    }
    if (currentPower > 0 && this->_rpm < 1000) {
      this->_notExpectedRotations++;
      if (this->_notExpectedRotations > 15) {
        AlarmData data;
        data.blocked = true;
        if (save(this->_filename, data)){
          char messageBuf[50];
          sprintf(messageBuf, "WARN Motor %s is blocked!", this->_filename);
          this->debugger->debug(messageBuf);
        } else {
          char messageBuf[50];
          sprintf(messageBuf, "ERR Motor %s is blocked and unable to save!", this->_filename);
          this->debugger->debug(messageBuf);
        }
        this->_stopped = true;
        this->_stoppedSince = millis();
        this->_reason = MOTOR_BLOCKED_REASON;
      }
    } else {
      this->_notExpectedRotations = 0;
    }
    return true;
  }
  return false;
}

bool RPMChecker::shouldStop() {
  if (this->_activated) {
    return this->_stopped || this->_overheated;
  }
  return this->_overheated;
}

bool RPMChecker::resetAlarm() {
  AlarmData data;
  if (save(this->_filename, data)) {
    this->_stopped = false;
    this->_notExpectedRotations = 0;
    return true;
  }
  return false;
}

void RPMChecker::report(AlarmReport * out) {
  if (!this->_activated) {
    return;
  }
  if (this->_stopped) {
    out->blocked = this->_reason == MOTOR_BLOCKED_REASON;
    out->highRpm = this->_reason == MOTOR_HIGH_RPM_REASON;
    out->overHeated = this->_overheated;
  }
  out->needAttention = this->_stopped;
  int durationPassed = (millis() - this->_stoppedSince);
  out->remainMinutes = (this->_unblockingFansPeriod - durationPassed) / 1000 / 60;
}