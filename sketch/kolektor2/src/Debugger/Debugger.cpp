#include "Arduino.h"

#include "ArduinoJson.h"
#include "Debugger.h"
#include "../Constants/Constants.h"
#include "../TimeProvider/TimeProvider.h"
#include "TimeLib.h"

Debugger::Debugger(TimeProvider * timeProvider) {
  this->_timeProvider = timeProvider;
  for (int i = 0; i< DEBUGGER_MESSAGES_COUNT; i++) {
      this->_messages[i][0] = '\0';
  }
}

void Debugger::debug(const char * message) {
    if (IS_DEBUG) Serial.println(message);
    if (this->_timeProvider->isTimeSet()) {
        char hourBuffer[4];
        if (hour() < 10) {
            sprintf(hourBuffer, "0%i", (int) hour());
        } else {
            sprintf(hourBuffer, "%i", (int)hour());
        }
        char minuteBuffer[4];
        if (minute() < 10) {
            sprintf(minuteBuffer, "0%i", (int)minute());
        } else {
            sprintf(minuteBuffer, "%i", (int)minute());
        }
        char messageBuf[DEBUGGER_MESSAGE_LENGTH];
        sprintf(messageBuf, "%d:%d %s", hourBuffer, minuteBuffer, message);
        strcpy(this->_messages[this->_index], messageBuf);
    } else {
        strcpy(this->_messages[this->_index], message);
    }
    this->_index = (this->_index + 1) % DEBUGGER_MESSAGES_COUNT; 
}

void Debugger::enable() {
    this->_enabled = true;
}

void Debugger::disable() {
    this->_enabled = false;
    for (int i = 0; i < DEBUGGER_MESSAGES_COUNT; i++) {
        // this->_messages[i] = "";
    }
    this->_index = 0;
}

void Debugger::getMessages(JsonArray * messages) {
    short actualIndex = this->_index;
    for (int i = 0; i < DEBUGGER_MESSAGES_COUNT; i++) {
        actualIndex = (actualIndex + 1) % DEBUGGER_MESSAGES_COUNT;
        messages->add(this->_messages[actualIndex]);
    }
}