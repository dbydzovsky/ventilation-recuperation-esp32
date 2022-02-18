#include "Arduino.h"
#include "Restarter.h"



void Restarter::act() {
  if (this->_shouldRestart) {
    ESP.restart();
  }
}

void Restarter::restart() {
  this->_shouldRestart = true;
}