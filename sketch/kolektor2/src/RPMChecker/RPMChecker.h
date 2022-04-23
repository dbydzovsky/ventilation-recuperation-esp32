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
#define MOTOR_OVER_HEATED 3
#include "../Debugger/Debugger.h"

struct AlarmReport {
  bool needAttention = false;
  bool highRpm = false;
  bool overHeated = false;
  bool blocked = false;
  long remainMinutes = 0;
};

class RPMChecker {
  public:
    RPMChecker(int pin, const char* filename, Debugger * debugger);
    void setup();
    void setTicksPerRevolution(short ticksPerRevolution);
    void deactivate();
    void setUnblockingFansPeriod(int unblockingPeriodSeconds);
    void setMaxRpm(int maxRpm);
    bool act(long ticks, short currentPower);
    bool actMaxTemp(int actualTemp, short currentPower);
    bool shouldStop();
    bool resetAlarm();
    void setMaxTemperature(int maxTemp);
    int getRpm();
    void report(AlarmReport * out);
  private:
    bool _checkMaxTemp = false;
    bool _overtempInitialized = false;
    unsigned long _overTempSince;
    int _maxTemp;
    bool _overheated = false;

    Debugger * debugger;
    short _ticksPerRevolution;
    int _unblockingFansPeriod = 0;
    bool _activated = true;
    bool _stopped = false;
    unsigned long _stoppedSince;
    int _pin;
    int _maxRpm;
    const char* _filename;
    int _reason = 0;
    int _notExpectedRotations = 0;
    int _rpm = 0;
    short _lastPower = 0;
    unsigned long last_sample = millis();
};

#endif