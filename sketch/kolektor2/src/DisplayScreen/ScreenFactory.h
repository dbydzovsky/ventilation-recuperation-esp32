#ifndef DisplayScreen_h
#define DisplayScreen_h
#include "Arduino.h"

#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"
#include <Adafruit_SSD1306.h>

struct ScreenProps {
    Dependencies * deps;
    Orchestrator * orchestrator;
    Adafruit_SSD1306 * d;
};

class Screen {
    public:
        virtual void setup(ScreenProps * deps);
        virtual bool isFinished(ScreenProps * deps) = 0;
        virtual void finish() = 0;
        virtual void tick(ScreenProps * deps) = 0;
        virtual bool canBeDimmed(ScreenProps * deps) = 0;
        virtual bool shouldShowScreenSaver(ScreenProps * deps) = 0;
        virtual int getDelayMs(ScreenProps * deps) = 0;
        virtual bool handleClick(ScreenProps * deps, byte times) = 0;
        virtual bool handleHold(ScreenProps * deps, int duration_ms, bool finished) = 0;
        virtual void onPressDown(ScreenProps * deps) = 0;
        virtual bool hasActiveButton() = 0;
};

class HistoryScreen: public Screen {
  public:
    virtual void updateHistory(ScreenProps * props) = 0;
};
class PasswordScreen: public Screen {
  public:
    virtual void setPass(long pass) = 0;
};

class ScreenFactory {
  public:
    ScreenFactory();
    Screen * logoScreen;
    Screen * mainScreen;
    Screen * pressButtonScreen;
    Screen * wifiScreen;
    Screen * trialScreen;
    Screen * screenSaverScreen;
    
    Screen * disableScreen;
    Screen * boostScreen;
    Screen * inHouseScreen;
    Screen * outHouseScreen;
    PasswordScreen * debugScreen;
    HistoryScreen * co2History;
    HistoryScreen * tempHistory;
    // Screen * ManualControlScreen;
};

#endif