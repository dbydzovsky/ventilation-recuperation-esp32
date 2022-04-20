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


void Monitoring::hideInternalTempHum() {
    this->_hideInternalTempHum = true;
}


void Monitoring::hideCo2() {
    this->_hideCo2 = true;
}

void Monitoring::doReport() {
    if (WiFi.status() != WL_CONNECTED) {
        if (IS_DEBUG) Serial.println("Wifi is not connected to send sensor statistics");
        return;
    }
    if (!this->_deps->httpLock->readLock()) {
        this->_deps->debugger->debug("Unable to send sensor statistics. Unable to get HTTP read lock.");
        return;
    }
    if (!this->_deps->confLock->readLock()) {
        this->_deps->httpLock->readUnlock();
        this->_deps->debugger->debug("Unable to send sensor statistics. Unable to get CONF read lock.");
        return;
    }
    StaticJsonDocument<1024> doc;
    JsonObject data = doc.createNestedObject("data");

    if (this->_deps->outsideTemp->isInitialized()) {
        data["Teplota"][0]["value"] = this->_deps->outsideTemp->getAverage();
    }
    if (this->_deps->outsideHum->isInitialized()) {
        data["Vlhkost"][0]["value"] = this->_deps->outsideHum->getAverage();
    }
    if (!this->_hideCo2) {
        float insideCo2 = this->_deps->co2Inside->getAverage();
        if (!isnan(insideCo2)) {
            data["CO2"][0]["value"] = insideCo2;
        }    
    }
    data["VentilatorPower"][0]["value"] = this->_deps->ventilation->getPower();
    data["VentilatorBlocked"][0]["value"] = this->_deps->ventilatorChecker->shouldStop();
    if (IS_RECUPERATION_ENABLED) { // todo make it configurable by settings
        data["RecuperationPower"][0]["value"] = this->_deps->recuperation->getPower();
        data["RecuperationBlocked"][0]["value"] = this->_deps->recuperationChecker->shouldStop();
    }
    data["Code"][0]["value"] = this->_orchestrator->getProgrammeCode() / 10;
    if (!this->_hideInternalTempHum) {
        if (this->_deps->insideTemp->isInitialized()) {
            data["TeplotaVnitrni"][0]["value"] = this->_deps->insideTemp->getAverage();
        }
        if (this->_deps->insideHum->isInitialized()) {
            data["VlhkostVnitrni"][0]["value"] = this->_deps->insideHum->getAverage();
        }    
    } 
    float dewPoint = this->_deps->dewPoint->getDewPoint();
    if (!isnan(dewPoint)) {
        data["RosnyBod"][0]["value"] = dewPoint;
    }
    data["Heap"][0]["value"] = ESP.getFreeHeap();
    data["Forecast"][0]["value"] = (int) this->_deps->forecast->howDoesItFeelLike();
    data["ForecastStatus"][0]["value"] = this->_deps->forecast->getLastStatusCode();
    // data["Warnings"][0]["value"] = (outsideTemperatureSensor->getWarnings() + outsideHumiditySensor->getWarnings() + insideTemperatureSensor->getWarnings());
    // data["Errors"][0]["value"] = (outsideTemperatureSensor->getErrors() + outsideHumiditySensor->getErrors() + insideTemperatureSensor->getErrors());
    char requestBody[1024];
    serializeJson(doc, requestBody);
    char url[110] = "";
    if (IS_DEBUG) Serial.println(url);
    sprintf(url, "https://iotplotter.com/api/v2/feed/%s", this->_deps->conf->getData()->monitoring->feed);
    HTTPClient * httpClient = this->_deps->httpClient;
    // dynamic_cast<WiFiClient&>(secureClient), 
    httpClient->begin(url);
    httpClient->addHeader("api-key", this->_deps->conf->getData()->monitoring->apikey);
    int responseCode = httpClient->POST(requestBody);
    httpClient->end();
    if (responseCode < 200 || responseCode > 299) {
        char messageBuf[50];
        sprintf(messageBuf, "Unexpected code from monitoring server %d", responseCode);
        this->_deps->debugger->debug(messageBuf);
    }
    this->_deps->httpLock->readUnlock();
    this->_deps->confLock->readUnlock();
}

void Monitoring::act() {
    if (millis() - this->last_retrival > updatingSensorStatsInterval) {
        this->last_retrival = millis();
        this->doReport();
    }
}