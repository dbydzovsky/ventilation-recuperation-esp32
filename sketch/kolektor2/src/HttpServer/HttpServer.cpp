#include "Arduino.h"
#include "ArduinoJson.h"
#include "HttpServer.h"
#include "../RGBDiode/RGBDiode.h"
#include <WiFiClientSecure.h>
#include "AsyncJson.h"
#include <ESPAsyncWebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include "../Constants/Constants.h"
#include "../Programme/Programme.h"
#include "../Orchestrator/Orchestrator.h"
#include "../FilterMonitor/FilterMonitor.h"
#include "SPIFFS.h"
#include "TimeLib.h"
#include "../RPMChecker/RPMChecker.h"

void setCors(AsyncWebServerResponse *response) {
  response->addHeader("Access-Control-Allow-Origin", "*");
}
void setCache(AsyncWebServerResponse *response) {
  response->addHeader("Cache-Control", "public, max-age=31536000, immutable");
}

void notNotFound(AsyncWebServerRequest * request) {
    if (request->method() == HTTP_OPTIONS) {
        AsyncWebServerResponse *response = request->beginResponse(204);
        setCors(response);
        setCache(response);
        request->send(response);
    } else if (request->method() == HTTP_GET) {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html");
        setCache(response);
        request->send(response);
    }
}

HttpServer::HttpServer(Dependencies * deps, AsyncWebServer * server,AsyncWiFiManager *wifiManager, Orchestrator * orchestrator,FilterMonitor * filter){
    this->_deps = deps;
    this->_server = server;
    this->_wifiManager = wifiManager;
    this->_orchestrator = orchestrator;
    this->_filter = filter;
}


void HttpServer::setup() {
    if (this->_deps->conf->dataSet && this->_deps->confLock->readLock()) {
        WiFi.hostname(this->_deps->conf->getData()->name);
        this->_deps->confLock->readUnlock();
    } else {
        WiFi.hostname("rekuperace");
    }
    this->_deps->diode->configure(&tickingWiFiConnecting);
    this->_wifiManager->setConfigPortalTimeout(180); // 3 minutes to be configured, then continue
    this->_wifiManager->autoConnect(DEFAULT_SSID, DEFAULT_PASSWORD);
    
    this->_server->on("/a/s/", HTTP_GET, [this](AsyncWebServerRequest * request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        ConfigurableProgramme * trialProgramme = this->_deps->factory->Trial;
        StaticJsonDocument<2048> jsonDoc;
        JsonObject root = jsonDoc.to<JsonObject>();
        JsonObject trial = root.createNestedObject("trial");
        byte mode = INACTIVE_MODE;
        if (this->_deps->conf->dataSet && this->_deps->confLock->readLock()) {
            mode = this->_deps->conf->getData()->mode;
            trial["enabled"] = trialProgramme->isValid(this->_deps->conf->getData());
            this->_deps->confLock->readLock();
        }
        root["mode"] = mode;
        if (this->_deps->timeProvider->isTimeSet()) {
            char timeBuffer[40];
            sprintf(timeBuffer, "%d/%d/%d %d:%d.%d", year(), month(), day(), hour(), minute(), second());
            root["time"] = timeBuffer;
        } else {
            root["time"] = "Čas nenastaven.";
        }
        root["ventilator"] = this->_deps->ventilation->getPower();
        root["ventilatorRPM"] = this->_deps->ventilatorChecker->getRpm();
        root["recuperation"] = this->_deps->recuperation->getPower();
        root["recuperationRPM"] = this->_deps->recuperationChecker->getRpm();
        root["alive"] = millis();
        char description[80] = "";
        this->_orchestrator->getProgrammeName(description);
        root["description"] = description;
        // todo root["restarts"] = restarts;
        root["heap"] = ESP.getFreeHeap();
        
        trial["duration"] = trialProgramme->getDuration();
        PowerOutput trialOutput = trialProgramme->getActualSetting();
        trial["mode"] = trialOutput.mode; 
        trial["ventilator"] = trialOutput.ventilatorPower;
        trial["recuperation"] = trialOutput.recuperationPower;
        trial["recuperationMode"] = trialOutput.recuperationMode;
        JsonObject insideTemp = root.createNestedObject("insideTemp");
        insideTemp["v"] = this->_deps->insideTemp->getValue();
        insideTemp["avg"] = this->_deps->insideTemp->getAverage();
        insideTemp["err"] = this->_deps->insideTemp->getErrors();
        insideTemp["warn"] = this->_deps->insideTemp->getWarnings();
        JsonObject co2Inside = root.createNestedObject("co2Inside");
        co2Inside["v"] = this->_deps->co2Inside->getValue();
        co2Inside["avg"] = this->_deps->co2Inside->getAverage();
        co2Inside["err"] = this->_deps->co2Inside->getErrors();
        co2Inside["warn"] = this->_deps->co2Inside->getWarnings();    
        JsonObject outsideTemp = root.createNestedObject("outsideTemp");
        outsideTemp["v"] = this->_deps->outsideTemp->getValue();
        outsideTemp["avg"] = this->_deps->outsideTemp->getAverage();
        outsideTemp["err"] = this->_deps->outsideTemp->getErrors();
        outsideTemp["warn"] = this->_deps->outsideTemp->getWarnings();
        JsonObject outsideHum = root.createNestedObject("outsideHum");
        outsideHum["v"] = this->_deps->outsideHum->getValue();
        outsideHum["avg"] = this->_deps->outsideHum->getAverage();
        outsideHum["err"] = this->_deps->outsideHum->getErrors();
        outsideHum["warn"] = this->_deps->outsideHum->getWarnings();

        JsonObject filterVentilator = root.createNestedObject("filterVentilator");
        FilterReport ventReport;
        this->_filter->report(FAN_TYPE_VENTILATOR, &ventReport);
        filterVentilator["needCleaning"] = ventReport.needCleaning;
        filterVentilator["remainingMinutes"] = ventReport.remainMinutes;
        
        JsonObject filterRecuperation = root.createNestedObject("filterRecuperation");
        FilterReport recuperationReport;
        this->_filter->report(FAN_TYPE_RECUPERATION, &recuperationReport);
        filterRecuperation["needCleaning"] = recuperationReport.needCleaning;
        filterRecuperation["remainingMinutes"] = recuperationReport.remainMinutes;
        
        JsonObject alarmVentilator = root.createNestedObject("alarmVentilator");
        AlarmReport ventilatorAlarmReport;
        this->_deps->ventilatorChecker->report(&ventilatorAlarmReport);
        alarmVentilator["needAttention"] = ventilatorAlarmReport.needAttention;
        alarmVentilator["blocked"] = ventilatorAlarmReport.blocked;
        alarmVentilator["highRpm"] = ventilatorAlarmReport.highRpm;
        alarmVentilator["remainMinutes"] = ventilatorAlarmReport.remainMinutes;
        
        JsonObject alarmRecuperation = root.createNestedObject("alarmRecuperation");
        AlarmReport recuperationAlarmReport;
        this->_deps->recuperationChecker->report(&recuperationAlarmReport);
        alarmRecuperation["needAttention"] = recuperationAlarmReport.needAttention;
        alarmRecuperation["blocked"] = recuperationAlarmReport.blocked;
        alarmRecuperation["highRpm"] = recuperationAlarmReport.highRpm;
        alarmRecuperation["remainMinutes"] = recuperationAlarmReport.remainMinutes;

        serializeJson(root, *response);
        setCors(response);
        request->send(response);
    });
    this->_server->on("/a/conf/", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncWebServerResponse * response = request->beginResponse(SPIFFS, "/config.json", "application/json");
        setCors(response);
        request->send(response);
    });
    this->_server->on("/a/settings/", HTTP_GET, [](AsyncWebServerRequest * request) {
        if (SPIFFS.exists("/settings.json")) {
            AsyncWebServerResponse * response = request->beginResponse(SPIFFS, "/settings.json", "application/json");
            setCors(response);
            request->send(response);
        } else {
            AsyncWebServerResponse *response = request->beginResponse(503, "application/json", "{'msg':'Settings file is not present'}");
            setCors(response);
            request->send(response);
        }
        
    });
    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/a/t", [this](AsyncWebServerRequest * request, JsonVariant & json) {
        if (json["on"].as<int>() == 1) {
            byte ventilatorPower = json["ventilator"].as<byte>();
            byte recuperationPower = json["recuperation"].as<byte>();
            byte recuperationMode = json["recuperationMode"].as<byte>();
            int duration = json["duration"].as<int>();
            ConfigurableProgramme * trialProgramme = this->_deps->factory->Trial;
            PowerOutput powerOutput;
            powerOutput.mode = POWER_OUTPUT_MODE_BOTH;
            powerOutput.ventilatorPower = ventilatorPower;
            powerOutput.recuperationPower = recuperationPower;
            powerOutput.recuperationMode = recuperationMode;
            trialProgramme->setPower(powerOutput, duration);
            this->_orchestrator->setProgramme(dynamic_cast<Programme*>(trialProgramme));
        } else {
            ConfigurableProgramme * trialProgramme = this->_deps->factory->Trial;
            PowerOutput powerOutput;
            trialProgramme->setPower(powerOutput, 0);
            trialProgramme->invalidate();
        }
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{'msg':'done'}");
        setCors(response);
        request->send(response);
    });

    AsyncCallbackJsonWebHandler* filterHandler = new AsyncCallbackJsonWebHandler("/a/clean/", [this](AsyncWebServerRequest * request, JsonVariant & json) {
        int fanType = json["filter"].as<int>();
        if (this->_filter->cleared(fanType)){
            AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{'msg':'done'}");
            setCors(response);
            request->send(response);    
        } else {
            AsyncWebServerResponse *response = request->beginResponse(503, "application/json", "{'msg':'error'}");
            setCors(response);
            request->send(response);
        }
    });
    AsyncCallbackJsonWebHandler* alarmHandler = new AsyncCallbackJsonWebHandler("/a/alarm/", [this](AsyncWebServerRequest * request, JsonVariant & json) {
        int fanType = json["filter"].as<int>();
        RPMChecker * checker;
        if (fanType == FAN_TYPE_VENTILATOR) {
            checker = this->_deps->ventilatorChecker;
        } else if (fanType == FAN_TYPE_RECUPERATION) {
            checker = this->_deps->recuperationChecker;
        } else {
            AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{'msg':'bad request'}");
            setCors(response);
            request->send(response);    
            return;
        }
        if (checker->resetAlarm()){
            AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{'msg':'done'}");
            setCors(response);
            request->send(response);    
        } else {
            AsyncWebServerResponse *response = request->beginResponse(503, "application/json", "{'msg':'error'}");
            setCors(response);
            request->send(response);
        }
    });
    AsyncCallbackJsonWebHandler* saveConfigHandler = new AsyncCallbackJsonWebHandler("/a/conf", [this](AsyncWebServerRequest * request, JsonVariant & json) {
        // todo lock?
        if (this->_deps->conf->save(json)) {
            AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{'msg':'done'}");
            setCors(response);
            request->send(response);
        } else {
            AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{'msg':'Could not parse JSON or not valid values'}");
            setCors(response);
            request->send(response);
        }
    });

    AsyncCallbackJsonWebHandler* saveSettingsHandler = new AsyncCallbackJsonWebHandler("/a/settings", [this](AsyncWebServerRequest * request, JsonVariant & json) {
        if (this->_deps->settings->save(json)) {
            AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{'msg':'done'}");
            setCors(response);
            request->send(response);
        } else {
            AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{'msg':'Could not parse JSON or not valid values'}");
            setCors(response);
            request->send(response);
        }
    });
    this->_server->addHandler(handler);
    this->_server->addHandler(saveConfigHandler);
    this->_server->addHandler(saveSettingsHandler);
    this->_server->addHandler(filterHandler);
    this->_server->addHandler(alarmHandler);
    this->_server->onNotFound(notNotFound);
    this->_server->begin();
}
