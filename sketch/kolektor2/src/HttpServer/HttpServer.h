/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef HttpServer_h
#define HttpServer_h
#include "Arduino.h"
#include "../Dependencies/Dependencies.h"
#include <ESPAsyncWebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include "../Orchestrator/Orchestrator.h"
#include "../FilterMonitor/FilterMonitor.h"

class HttpServer {
  public:
    HttpServer(Dependencies * deps, AsyncWebServer * server,AsyncWiFiManager *wifiManager, Orchestrator * orchestrator, FilterMonitor * filter);
    void setup();
  private:
    Dependencies * _deps;
    AsyncWebServer * _server;
    AsyncWiFiManager * _wifiManager;
    Orchestrator * _orchestrator;
    FilterMonitor * _filter;
};

#endif