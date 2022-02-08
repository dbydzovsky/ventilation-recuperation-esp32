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

RPMChecker::RPMChecker(int pin, int maxRpm, const char* filename){
  this->_pin = pin;
  this->_maxRpm = maxRpm;
  this->_filename = filename;
}

void RPMChecker::setup() {
  AlarmData data;
  read(this->_filename, &data);
  if (data.blocked || data.highRpm) {
    this->_blocked = true;
    if (data.blocked) {
      this->_reason = MOTOR_BLOCKED_REASON;
    }
    if (data.highRpm) {
      this->_reason = MOTOR_HIGH_RPM_REASON;
    }
    this->_blockedSince = millis();
  }
}

void RPMChecker::act(short currentPower) {
  // todo act
  // persistBlocked(this->_filename);
}

bool RPMChecker::shouldStop() {
  return this->_blocked;
}

bool RPMChecker::resetAlarm() {
  AlarmData data;
  if (save(this->_filename, data)) {
    this->_blocked = false;
    return true;
  }
  return false;
}

void RPMChecker::report(AlarmReport * out) {
  if (this->_blocked) {
    out->blocked = this->_reason == MOTOR_BLOCKED_REASON;
    out->highRpm = this->_reason == MOTOR_HIGH_RPM_REASON;
  }
  out->needAttention = this->_blocked;
  // todo compute
  out->remainMinutes = 0;
}