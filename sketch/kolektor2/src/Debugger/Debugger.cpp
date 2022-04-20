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
// todo add time to message
//    char hourBuffer[3];
//    char minuteBuffer[3];
//    bool timeset = this->_timeProvider->isTimeSet();
//    if (timeset) {
//        if (hour() < 10) {
//            sprintf(hourBuffer, "0%d", hour());
//        } else {
//            sprintf(hourBuffer, "%d", hour());
//        }
//        if (minute() < 10) {
//            sprintf(minuteBuffer, "0%d", minute());
//        } else {
//            sprintf(minuteBuffer, "%d", minute());
//        }
//    }
//    char messageBuf[DEBUGGER_MESSAGE_LENGTH+5];
//    sprintf(messageBuf, "%d:%d %s", hourBuffer, minuteBuffer, this->_messages[actualIndex]);
//    messages->add(messageBuf);
    strcpy(this->_messages[this->_index], message);
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