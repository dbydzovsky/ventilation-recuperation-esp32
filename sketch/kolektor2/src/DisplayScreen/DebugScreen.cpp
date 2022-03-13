#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenFactory.h"
#include "TimeLib.h"
#define OLED_RESET 0
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"
#include "../Configuration/Configuration.h"
#include "../Constants/Constants.h"
#include "../Weather/Weather.h"
#include "../Settings/Settings.h"
#include "../Programme/Programme.h"


class DebugScreen: public Screen {
  private:
    unsigned long opened_since;
  public:
    void setup(ScreenProps * deps){
        this->opened_since = millis();
      if (IS_DEBUG) Serial.println("Debug screen setup");
    }
    bool isFinished(ScreenProps * props) {
      return millis() - this->opened_since > KEEP_SCREEN_LONG;
    }
    void finish() {
      
    };
    void tick(ScreenProps * props){ 
      props->d->clearDisplay();
      props->d->setTextSize(2);
      props->d->setTextColor(WHITE);
      props->d->setCursor(0,0);
      props->d->print("Debug");
      props->d->setTextSize(1);
      props->d->setCursor(0, 16);
      int code = props->orchestrator->getProgrammeCode();
      props->d->print(code);
      props->d->print("-");
      props->d->print(ESP.getFreeHeap());
      props->d->setCursor(2, 24);
      unsigned long secondsAlive = millis() / 1000;
      int days = secondsAlive / 86400; // a day
      int hours = (secondsAlive / 3600) % 24;
      int minutes = (secondsAlive / 60) % 60;
      props->d->print(days);
      props->d->print("d ");
      props->d->print(hours);
      props->d->print(":");
      props->d->print(minutes);

      props->d->setCursor(2, 34);
      props->d->print("v1.0 @2022");
      props->d->display();
    }
    bool canBeDimmed(ScreenProps * deps) {
      return false;
    }
    int getDelayMs(ScreenProps * deps) {
      return 1000;
    }
    bool handleClick(ScreenProps * deps, byte times){
      return false;
    }

    bool handleHold(ScreenProps * props, int duration_ms, bool finished){
      return true;
    }

    void onPressDown(ScreenProps * deps) {

    }

    bool hasActiveButton() {
      return false;
    }
};
