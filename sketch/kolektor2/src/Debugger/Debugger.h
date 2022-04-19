


#ifndef Debugger_h
#define Debugger_h
#include "Arduino.h"
#include "ArduinoJson.h"

#define DEBUGGER_MESSAGES_COUNT 20

class Debugger {
  public:
    void enable();
    void disable();
    void debug(const char* message);
    void getMessages(JsonArray * messages);
  private:
    short _index = 0;
    const char *_messages[DEBUGGER_MESSAGES_COUNT];
    bool _enabled = true;
};


#endif