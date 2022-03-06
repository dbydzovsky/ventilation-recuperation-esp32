#ifndef RGBDiode_h
#define RGBDiode_h
#include "Arduino.h"
#include <Ticker.h>

#define RGBDIODE_CLASSICAL_MODE 0
#define RGBDIODE_PRIORITY_MODE 1

#define DIODE_NONE_COLOR 0
#define DIODE_RED_COLOR 1
#define DIODE_GREEN_COLOR 2
#define DIODE_BLUE_COLOR 3

typedef struct {
  byte ticking[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte size = 0;
} TickingConfiguration;

class RGBDiode {
  public:
    RGBDiode(int channel, int colorA, int colorB, int colorC);
    void detach();
    void configure(void (*configuration)(TickingConfiguration *out));
    void important(void (*configuration)(TickingConfiguration *out));
    void setBrightness(int brightness);
  private:
    Ticker ticker;
    static void staticTickerCallbackLed(RGBDiode *pThis);
    void set(void (*configuration)(TickingConfiguration *out));
    void tick();
    void aOn();
    void bOn();
    void cOn();
    void off();
    int _a;
    int _b;
    int _c;
    int _channel;
    int _brightness = 1024;
    byte _previous = 0;
    byte _tickingConf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    byte _tickingIndex = 0;
    byte _mode = RGBDIODE_CLASSICAL_MODE;
    byte _tickingSize = 0;
    void (*_configuration)(TickingConfiguration*);
};


void tickingWiFiConnecting(TickingConfiguration *out);
void tickingWarmingUp(TickingConfiguration *out);
void tickingTrial(TickingConfiguration *out);
void tickingDeviceEnabledWifiOn(TickingConfiguration *out);
void tickingDeviceEnabledWifiOff(TickingConfiguration *out);
void tickingError(TickingConfiguration *out);
void tickingEnablementRed(TickingConfiguration *out);
void tickingCritical(TickingConfiguration *out);
void tickingConditionsMet(TickingConfiguration *out);
void tickingSummerDeviceEnabledWifiOn(TickingConfiguration *out);
void tickingSummerDeviceEnabledWifiOff(TickingConfiguration *out);
void tickingAutoProgrammeOn(TickingConfiguration *out);
void tickingDeviceDisabledWifiOn(TickingConfiguration *out);
void tickingDeviceDisabledWifiOff(TickingConfiguration *out);
void tickingRecuperation(TickingConfiguration *out);
#endif