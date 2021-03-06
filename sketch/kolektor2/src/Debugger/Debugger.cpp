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

void Debugger::trace(const char *message) {
    if (this->_traceEnabled) {
        this->debug(message);
    }
}

void Debugger::setTrace(bool enabled) {
    if (enabled != this->_traceEnabled) {
        if (enabled) {
            this->debug("Enabling TRACE logging.");
        } else {
            this->debug("Disabling TRACE logging.");
        }
    }
    this->_traceEnabled = enabled;
}
bool Debugger::isTraceEnabled(){
    return this->_traceEnabled;
}
void Debugger::debug(const char * message) {
    if (!this->_enabled) {
        return;
    }
    if (IS_DEBUG) Serial.println(message);
    if (this->_timeProvider->isTimeSet()) {
        char hourBuffer[3];
        if (hour() < 10) {
            sprintf(hourBuffer, "0%d", hour());
        } else {
            sprintf(hourBuffer, "%d", hour());
        }
        char minuteBuffer[3];
        if (minute() < 10) {
            sprintf(minuteBuffer, "0%d", minute());
        } else {
            sprintf(minuteBuffer, "%d", minute());
        }
        char messageBuf[DEBUGGER_MESSAGE_LENGTH];
        sprintf(messageBuf, "%d.%d-%s:%s %s", day(), month(), hourBuffer, minuteBuffer, message);
        strcpy(this->_messages[this->_index], messageBuf);
    } else {
        strcpy(this->_messages[this->_index], message);
    }
    this->_version = this->_version + 1;
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

void Debugger::printMessages(Print * output) {
    short actualIndex = this->_index;
    for (int i = 0; i < DEBUGGER_MESSAGES_COUNT; i++) {
        if (this->_messages[actualIndex][0] != '\0') {
            output->println(this->_messages[actualIndex]);
        }
        actualIndex = (actualIndex + 1) % DEBUGGER_MESSAGES_COUNT;
    }
}

void Debugger::getMessages(JsonArray * messages) {
    short actualIndex = this->_index;
    for (int i = 0; i < DEBUGGER_MESSAGES_COUNT; i++) {
        if (this->_messages[actualIndex][0] != '\0') {
            messages->add(this->_messages[actualIndex]);
        }
        actualIndex = (actualIndex + 1) % DEBUGGER_MESSAGES_COUNT;
    }
}

int Debugger::version() {
    return this->_version;
}