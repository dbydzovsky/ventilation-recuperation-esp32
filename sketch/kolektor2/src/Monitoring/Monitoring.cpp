#include "Arduino.h"
#include "ArduinoJson.h"
#include "Monitoring.h"
#include "../Constants/Constants.h"
#include <WiFiClientSecure.h>
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"

Monitoring::Monitoring(Orchestrator* orchestrator, Dependencies * deps){
  this->_deps = deps;
  this->_orchestrator = orchestrator;
}

void Monitoring::doReport() {
    if (WiFi.status() != WL_CONNECTED) {
        if (IS_DEBUG) Serial.println("Wifi is not connected to send sensor statistics");
    }
    if (!this->_deps->httpLock->readLock()) {
        if (IS_DEBUG) Serial.println("Unable to get http read lock to send sensor stats.");
    }
    if (!this->_deps->confLock->readLock()) {
        this->_deps->httpLock->readUnlock();
        if (IS_DEBUG) Serial.println("Unable to get conf read lock to send sensor stats.");
    }
    StaticJsonDocument<512> doc;
    JsonObject data = doc.createNestedObject("data");
    float outsideTemperature = this->_deps->outsideTemp->getAverage();
    if (!isnan(outsideTemperature)) {
        data["Teplota"][0]["value"] = outsideTemperature;
    }
    float outsideHumidity = this->_deps->outsideHum->getAverage();
    if (!isnan(outsideHumidity)) {
        data["Vlhkost"][0]["value"] = outsideHumidity;
    }
    float insideCo2 = this->_deps->co2Inside->getAverage();
    if (!isnan(insideCo2)) {
        data["CO2"][0]["value"] = insideCo2;
    }
    data["VentilatorPower"][0]["value"] = this->_deps->ventilation->getPower();
    data["VentilatorBlocked"][0]["value"] = this->_deps->ventilatorChecker->shouldStop();
    data["RecuperationPower"][0]["value"] = this->_deps->recuperation->getPower();
    data["RecuperationBlocked"][0]["value"] = this->_deps->recuperationChecker->shouldStop();
    data["Code"][0]["value"] = this->_orchestrator->getProgrammeCode() / 10;
    float insideTemperature = this->_deps->insideTemp->getAverage();
    if (!isnan(insideTemperature)) {
        data["TeplotaVnitrni"][0]["value"] = insideTemperature;
    }
    float dewPoint = this->_deps->dewPoint->getDewPoint();
    if (!isnan(dewPoint)) {
        data["RosnyBod"][0]["value"] = dewPoint;
    }
    data["Heap"][0]["value"] = ESP.getFreeHeap();
    // data["Warnings"][0]["value"] = (outsideTemperatureSensor->getWarnings() + outsideHumiditySensor->getWarnings() + insideTemperatureSensor->getWarnings());
    // data["Errors"][0]["value"] = (outsideTemperatureSensor->getErrors() + outsideHumiditySensor->getErrors() + insideTemperatureSensor->getErrors());
    char requestBody[1024];
    serializeJson(doc, requestBody);
    char url[100] = "";
    if (IS_DEBUG) Serial.println(url);
    sprintf(url, "https://iotplotter.com/api/v2/feed/%s", this->_deps->conf->getData()->monitoring->feed);
    HTTPClient * httpClient = this->_deps->httpClient;
    // dynamic_cast<WiFiClient&>(secureClient), 
    httpClient->begin(url);
    httpClient->addHeader("api-key", this->_deps->conf->getData()->monitoring->apikey);
    int responseCode = httpClient->POST(requestBody);
    httpClient->end();
    if (IS_DEBUG) Serial.print("Response code from sensor stat update: ");
    if (IS_DEBUG) Serial.println(responseCode);
    this->_deps->httpLock->readUnlock();
    this->_deps->confLock->readUnlock();
}

void Monitoring::act() {
    if (millis() - this->last_retrival > updatingSensorStatsInterval) {
        this->last_retrival = millis();
        this->doReport();
    }
}