#ifndef RGBDiode_h
#define RGBDiode_h
#include "Arduino.h"
#include <Ticker.h>

#define RGBDIODE_CLASSICAL_MODE 0
#define RGBDIODE_PRIORITY_MODE 1

class RGBDiode {
  public:
    RGBDiode(int colorA, int colorB, int colorC);
    void detach();
    void configure(byte tickingName, byte tickingSize, const byte ticking[8]);
    void important(byte tickingName, byte tickingSize, const byte ticking[8]);
  private:
    Ticker ticker;
    static void staticTickerCallbackLed(RGBDiode *pThis);
    void set(byte tickingName, byte tickingSize, const byte ticking[8]);
    void tick();
    void aOn();
    void bOn();
    void cOn();
    void off();
    int _a;
    int _b;
    int _c;
    byte _previous = 0;
    byte _tickingConf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    byte _tickingIndex = 0;
    byte _mode = RGBDIODE_CLASSICAL_MODE;
    byte _tickingSize = 0;
    byte _tickingName = 0;
};

#endif