


#ifndef Debugger_h
#define Debugger_h
#include "Arduino.h"
#include "ArduinoJson.h"
#include "../TimeProvider/TimeProvider.h"
#include "TimeLib.h"
#define DEBUGGER_MESSAGES_COUNT 100
#define DEBUGGER_MESSAGE_LENGTH 100

class Debugger {
  public:
    Debugger(TimeProvider * timeProvider);
    void enable();
    void disable();
    void setTrace(bool enabled);
    bool isTraceEnabled();
    void debug(const char* message);
    void trace(const char* message);
    void printMessages(Print * output);
    void getMessages(JsonArray * messages);
    int version();
  private:
    bool _traceEnabled = true;
    int _version = random(9999999);
    TimeProvider * _timeProvider;
    short _index = 0;
    char _messages[DEBUGGER_MESSAGES_COUNT][DEBUGGER_MESSAGE_LENGTH];
    bool _enabled = true;
};


#endif