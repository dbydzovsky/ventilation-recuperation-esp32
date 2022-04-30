#include "Arduino.h"
#include "ArduinoJson.h"
#include "Weather.h"
#include "HTTPClient.h"
#include <WiFiClientSecure.h>
#include "../Constants/Constants.h"
WeatherForecast::WeatherForecast() {

}
// todo kontrolovat ze se netoci, kdyz nema
bool WeatherForecast::sync(WeatherDeps * deps){ 
  deps->debugger->debug("Syncing weather forecast");
  if (WiFi.status() != WL_CONNECTED) {
    deps->debugger->trace("Wifi is not connected to gather actual forecast!");
    return false;
  }
  
  if (!deps->httpsLock->readLock()) {
    deps->debugger->trace("Unable to sync forecast. Unable to lock HTTP lock.");
    return false;
  }
  char url[220] = "";
  ConfigurationData * data = deps->data;
  sprintf(url, "https://api.openweathermap.org/data/2.5/onecall?lat=%s&lon=%s&exclude=minutely,hourly,alerts&units=metric&appid=%s", data->lat, data->lon, data->weatherApiKey);
  // HTTPClient httpClientForecast;
  // dynamic_cast<WiFiClient&>(secureClient), 
  HTTPClient * httpClientForecast = deps->httpClient;
  
  httpClientForecast->begin(url);
  this->last_retrival = millis();
  this->lastStatusCode = httpClientForecast->GET();
  
  if (this->lastStatusCode >= 200 && this->lastStatusCode < 300) {
    int contentLength = httpClientForecast->getSize();
    if (contentLength == -1) {
      contentLength = 10000;
    }
    DynamicJsonDocument doc(contentLength);
    deserializeJson(doc, httpClientForecast->getStream());
    int offset = doc["timezone_offset"].as<int>();
    unsigned long currentTime = doc["current"]["dt"].as<unsigned long>();
    unsigned long firstItemTime = doc["daily"][0]["dt"].as<unsigned long>();
    if (currentTime < firstItemTime) {
      this->feelsLikeTomorrow = doc["daily"][0]["feels_like"]["day"].as<float>();
    } else {
      this->feelsLikeTomorrow = doc["daily"][1]["feels_like"]["day"].as<float>();
    }
    char messageBuf[100];
    sprintf(messageBuf, "Updated forecast. Will feel like %d C", (int)this->feelsLikeTomorrow);
    deps->debugger->debug(messageBuf);
    deps->timeProvider->updateTime(currentTime, offset);
    char newTimeBuf[100];
    sprintf(newTimeBuf, "New time: %d, offset: %d", currentTime, offset);
    deps->debugger->debug(newTimeBuf);
    if (this->feelsLikeTomorrow > deps->data->minimumFeelsLike) {
      deps->debugger->debug("Tomorrow will be hot, I should cool tonight.");
    }
    this->last_success = millis();
    httpClientForecast->end();
    deps->httpsLock->readUnlock();
    return true;
  } else {
    char messageBuf[100];
    sprintf(messageBuf, "WARN Unexpected code from forecast server %d", this->lastStatusCode);
    deps->debugger->debug(messageBuf);
    httpClientForecast->end();
    deps->httpsLock->readUnlock();
    return false;
  }
}

float WeatherForecast::howDoesItFeelLike(){ 
  return this->feelsLikeTomorrow;
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
  return this->lastStatusCode >= 200 && this->lastStatusCode < 300 && (millis() - this->last_success < this->syncForecastTolerateLastSuccessFor);
}
void WeatherForecast::act(WeatherDeps * deps) {
  if ((this->lastStatusCode == -100) || (millis() - this->last_retrival > this->syncForecastInterval)) {
    deps->debugger->trace("It is time to sync forecast");
    this->sync(deps);
  }
  if (this->hasValidForecast()) {
    if (this->feelsLikeTomorrow > deps->data->minimumFeelsLike) {
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
