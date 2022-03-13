#include "Arduino.h"
#include "ArduinoJson.h"
#include "Weather.h"
#include "HTTPClient.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"
WeatherForecast::WeatherForecast() {

}

bool WeatherForecast::sync(WeatherDeps * deps){ 
  if (IS_DEBUG) Serial.println("Syncing forecast");
  if (WiFi.status() != WL_CONNECTED) {
    if (IS_DEBUG) Serial.println("Wifi is not connected to gather actual forecast!");
    return false;
  }
  if (!deps->httpsLock->readLock()) {
    if (IS_DEBUG) Serial.println("https lock not locked!");
    return false;
  }
  char url[220] = "";
  ConfigurationData * data = deps->data;
  sprintf(url, "https://api.openweathermap.org/data/2.5/onecall?lat=%s&lon=%s&exclude=minutely,hourly,alerts&units=metric&appid=%s", data->lat, data->lon, data->weatherApiKey);
  // HTTPClient httpClientForecast;
  // dynamic_cast<WiFiClient&>(secureClient), 
  HTTPClient * httpClientForecast = deps->httpClient;
  if (IS_DEBUG) Serial.println(url);
  httpClientForecast->begin(url);
  this->last_retrival = millis();
  this->lastStatusCode = httpClientForecast->GET();
  if (IS_DEBUG) Serial.println(this->last_success);
  if (this->lastStatusCode >= 200 && this->lastStatusCode < 300) {
    int contentLength = httpClientForecast->getSize();
    if (contentLength == -1) {
      contentLength = 6000;
    }
    DynamicJsonDocument doc(contentLength);
    deserializeJson(doc, httpClientForecast->getStream());
    int offset = doc["timezone_offset"].as<int>();
    unsigned long unixTime = doc["current"]["dt"].as<unsigned long>();
    deps->timeProvider->updateTime(unixTime, offset);
    this->feelsLikeToday = doc["daily"][0]["feels_like"]["day"].as<float>();
    if (IS_DEBUG) Serial.print("Feels like: ");
    if (IS_DEBUG) Serial.println(this->feelsLikeToday);
    this->last_success = millis();
    httpClientForecast->end();
    deps->httpsLock->readUnlock();
    return true;
  } else {
    httpClientForecast->end();
    deps->httpsLock->readUnlock();
    return false;
  }
}

float WeatherForecast::howDoesItFeelLike(){ 
  return this->feelsLikeToday;
}
short WeatherForecast::getLastStatusCode() {
  return this->lastStatusCode;
}
void WeatherForecast::setSyncInterval(int interval) {
  this->syncForecastInterval = interval;
}
void WeatherForecast::setTolerateLastSuccess(int interval) {
  this->syncForecastTolerateLastSuccessFor = interval;
}

bool WeatherForecast::hasValidForecast(){
  return !(this->lastStatusCode != -100) && (millis() - this->last_success > this->syncForecastTolerateLastSuccessFor);
}
void WeatherForecast::act(WeatherDeps * deps) {
  if ((this->lastStatusCode == -100) || (millis() - this->last_retrival > this->syncForecastInterval)) {
    if (IS_DEBUG) Serial.println("It is time to sync forecast");
    this->sync(deps);
  }
  if (this->hasValidForecast()) {
    if (this->feelsLikeToday > deps->data->minimumFeelsLike) {
      this->shouldCool = true;
    }
    return;
  } else {
    unsigned long since_last_success = millis() - this->last_success;
    if (since_last_success > this->syncForecastTolerateLastSuccessFor) {
      this->last_success = millis() - since_last_success;
    }
  }
  this->shouldCool = false;
}
bool WeatherForecast::shouldCoolInsides(){
  return this->shouldCool;
}
