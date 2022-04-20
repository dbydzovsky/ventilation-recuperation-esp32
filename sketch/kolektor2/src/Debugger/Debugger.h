


#ifndef Debugger_h
#define Debugger_h
#include "Arduino.h"
#include "ArduinoJson.h"
#include "../TimeProvider/TimeProvider.h"

#define DEBUGGER_MESSAGES_COUNT 100
#define DEBUGGER_MESSAGE_LENGTH 200

class Debugger {
  public:
    Debugger(TimeProvider * timeProvider);
    void enable();
    void disable();
    void debug(const char* message);
    void getMessages(JsonArray * messages);
  private:
    TimeProvider * _timeProvider;
    short _index = 0;
    char _messages[DEBUGGER_MESSAGES_COUNT][DEBUGGER_MESSAGE_LENGTH];
    bool _enabled = true;
};


#endif