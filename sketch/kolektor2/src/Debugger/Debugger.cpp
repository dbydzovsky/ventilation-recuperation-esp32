#include "Arduino.h"

#include "ArduinoJson.h"
#include "Debugger.h"
#include "../Constants/Constants.h"

void Debugger::debug(const char * message) {
    if (IS_DEBUG) Serial.println(message);
    this->_messages[this->_index] = message;
    this->_index = (this->_index + 1) % DEBUGGER_MESSAGES_COUNT; 
}

void Debugger::enable() {
    this->_enabled = true;
}

void Debugger::disable() {
    this->_enabled = false;
    for (int i = 0; i < DEBUGGER_MESSAGES_COUNT; i++) {
        this->_messages[i] = "";
    }
    this->_index = 0;
}

void Debugger::getMessages(JsonArray * messages) {
    short actualIndex = this->_index;
    messages->add("Messages:");
    for (int i = 0; i < DEBUGGER_MESSAGES_COUNT; i++) {
        actualIndex = (actualIndex + 1) % DEBUGGER_MESSAGES_COUNT;
        const char* message = this->_messages[actualIndex];
        if (message != "") {
           messages->add(message);
        }
    }
}