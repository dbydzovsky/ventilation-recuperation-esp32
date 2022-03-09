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


class ManualScreen: public Screen {
  public:
    void setup(ScreenProps * deps){
      if (IS_DEBUG) Serial.println("Manual screen setup");
    }
    bool isFinished(ScreenProps * props) {
      return props->deps->factory->Trial->isValid(props->deps->conf->getData());
    }
    void finish() {
      
    };
    void tick(ScreenProps * props){ 
      ConfigurableProgramme * trialProgramme = props->deps->factory->Trial;
      int remains = trialProgramme->remainingSeconds();
      // PowerOutput trialOutput = trialProgramme->getActualSetting();
      props->d->clearDisplay();
      // props->d->drawBitmap(24, 32, dring_icon16x16, 16, 16, WHITE);
      props->d->setTextSize(2);
      props->d->setTextColor(WHITE);
      props->d->setCursor(2,10);
      props->d->print("MAN.");
      props->d->setTextSize(1);
      props->d->setCursor(2, 28);
      int minutes = remains / 60;
      int seconds = remains % 60;
      props->d->print(minutes);
      props->d->print(" min");
      props->d->setCursor(2, 38);
      props->d->print(seconds);
      props->d->print(" sec");
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
      if (IS_DEBUG) Serial.println("Boosting..");
      ConfigurableProgramme * trialProgramme = props->deps->factory->Trial;
      trialProgramme->invalidate();
      return true;
    }

    void onPressDown(ScreenProps * deps) {

    }

    bool hasActiveButton() {
      return true;
    }
};
