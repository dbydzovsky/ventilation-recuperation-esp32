#include "Arduino.h"
#include "ArduinoJson.h"
#include "RPMChecker.h"
#include "../Constants/Constants.h"
#include "SPIFFS.h"

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

RPMChecker::RPMChecker(int pin, const char* filename){
  this->_pin = pin;
  this->_filename = filename;
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
      this->_reason = MOTOR_BLOCKED_REASON;
    }
    if (data.highRpm) {
      this->_reason = MOTOR_HIGH_RPM_REASON;
    }
    this->_stoppedSince = millis();
  }
}

float computeRpm(int durationMs, long ticks) {
  int revolutions = ticks / 2;
  float rpm = (60000/durationMs) * revolutions;
  return rpm;
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
  int duration = millis() - this->last_sample;
  if (duration > RPM_SAMPLING_DURATION) {
    this->last_sample = millis();
    this->_rpm = computeRpm(duration, ticks);
    if (this->_rpm > this->_maxRpm) {
      AlarmData data;
      data.highRpm = true;
      save(this->_filename, data);
      this->_stopped = true;
      this->_stoppedSince = millis();
      this->_reason = MOTOR_HIGH_RPM_REASON;
    }
    if (currentPower > 0 && this->_rpm < 1000) {
      this->_notExpectedRotations++;
      if (this->_notExpectedRotations > 5) {
        AlarmData data;
        data.blocked = true;
        save(this->_filename, data);
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
    return this->_stopped;
  }
  return false;
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
  }
  out->needAttention = this->_stopped;
  int durationPassed = (millis() - this->_stoppedSince);
  out->remainMinutes = (this->_unblockingFansPeriod - durationPassed) / 1000 / 60;
}