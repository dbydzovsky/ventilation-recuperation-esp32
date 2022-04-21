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

class OutHouseScreen: public Screen {
  private:
	unsigned long opened_since = millis(); 
  public:
    void setup(ScreenProps * deps){
	    this->opened_since = millis();
    }
    bool isFinished(ScreenProps * deps) {
      return millis() - this->opened_since > KEEP_SCREEN_LONG;
    }
    void finish() {
      
    };
    void tick(ScreenProps * props){ 
	    props->d->clearDisplay();
      props->d->setCursor(0, 0);
	    props->d->setTextSize(1);
      props->d->setTextColor(WHITE);
      props->d->print("OUT ");

      if (props->deps->forecast->hasValidForecast()) {
        props->d->print((int) props->deps->forecast->howDoesItFeelLike());
        props->d->print(" C");
      } else {
        props->d->print(" NaN");
      }
	    props->d->setCursor(2, 12);
	  
      props->d->print(props->deps->outsideTemp->getAverage());
      props->d->print(" C");
	    props->d->setCursor(2, 24);
	    props->d->print(props->deps->outsideHum->getAverage());
      props->d->print(" %");
	    props->d->setCursor(2, 36);
	    props->d->print((int) props->deps->dewPoint->getDewPoint());
      props->d->print(" rosny");
      props->d->display();
    }
    bool canBeDimmed(ScreenProps * deps) {
      return false;
    }
    bool shouldShowScreenSaver(ScreenProps * deps) {
      return true; // todo
    }
    int getDelayMs(ScreenProps * deps) {
      return 500;
    }
    bool handleClick(ScreenProps * deps, byte times){
      return false;
    }
    bool handleHold(ScreenProps * props, int duration_ms, bool finished){
      if (IS_DEBUG) Serial.println("Disabling...");
	  return true;
    }

    void onPressDown(ScreenProps * deps) {

    }

    bool hasActiveButton() {
      return false;
    }
};
