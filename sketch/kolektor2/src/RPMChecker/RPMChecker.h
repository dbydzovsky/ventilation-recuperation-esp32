/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef RPMChecker_h
#define RPMChecker_h
#include "Arduino.h"

#define MOTOR_NO_REASON 0
#define MOTOR_BLOCKED_REASON 1
#define MOTOR_HIGH_RPM_REASON 2
 
struct AlarmReport {
  bool needAttention = false;
  bool highRpm = false;
  bool blocked = false;
  long remainMinutes = 0;
};

class RPMChecker {
  public:
    RPMChecker(int pin, int maxRpm, const char* filename);
    void setup();
    void act(short currentPower);
    bool shouldStop();
    bool resetAlarm();
    void report(AlarmReport * out);
  private:
    bool _blocked = false;
    unsigned long _blockedSince;
    int _pin;
    int _maxRpm;
    const char* _filename;
    int _reason = 0;
};

#endif