#include "Arduino.h"
#include "Restarter.h"
#include "../FilterMonitor/FilterMonitor.h"

Restarter::Restarter(FilterMonitor * monitor) {
  this->_monitor = monitor;
}

void Restarter::act() {
  if (this->_shouldRestart) {
    this->_monitor->persist();
    ESP.restart();
  }
}

void Restarter::restart() {
  this->_shouldRestart = true;
}