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

// 'sfan16', 16x16px
const unsigned char fan1 [] PROGMEM = {
	0x03, 0xe0, 0x07, 0xf0, 0x07, 0xf0, 0x07, 0xf0, 0x77, 0xc0, 0xf3, 0xde, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x7b, 0xcf, 0x03, 0xe6, 0x0f, 0xe0, 0x0f, 0xe0, 0x0f, 0xe0, 0x07, 0xc0
};
// 'sfan16_45', 16x16px
const unsigned char fan2[] PROGMEM = {
	0x00, 0x00, 0x1c, 0x00, 0x3e, 0x3c, 0x3e, 0x7e, 0x3e, 0xfe, 0x3f, 0xfe, 0x1f, 0xfc, 0x0f, 0xe0, 
	0x07, 0xf0, 0x3f, 0xf8, 0x7f, 0xfc, 0x7f, 0x7c, 0x7e, 0x7c, 0x3c, 0x7c, 0x00, 0x38, 0x00, 0x00
};
class ManualScreen: public Screen {
  private:
    byte index = 0;
  public:
    void setup(ScreenProps * deps){
      deps->deps->debugger->trace("Manual screen setup");
      this->index = 0;
    }
    bool isFinished(ScreenProps * props) {
      return !props->deps->factory->Trial->isValid(props->deps->conf->getData());
    }
    void finish() {
      
    };
    void tick(ScreenProps * props){ 
      ConfigurableProgramme * trialProgramme = props->deps->factory->Trial;
      int remains = trialProgramme->remainingSeconds();
      PowerOutput trialOutput = trialProgramme->getActualSetting();

      props->d->clearDisplay();
      if (trialOutput.ventilatorPower != 0 || trialOutput.recuperationPower != 0) {
        if (this->index % 2 == 1) {
          props->d->drawBitmap(48, 0, fan1, 16, 16, WHITE);
        } else {
          props->d->drawBitmap(48, 0, fan2, 16, 16, WHITE);
        }
      } else {
        props->d->drawBitmap(48, 0, fan1, 16, 16, WHITE);
      }
      this->index = (this->index + 1) % 2;
      props->d->setCursor(0,0);
      props->d->setTextSize(1);
      props->d->setTextColor(WHITE);
      props->d->print("Manualni");
      props->d->setTextSize(1);
      props->d->setCursor(2, 28);
      int minutes = remains / 60;
      int seconds = remains % 60;
      props->d->print(minutes);
      props->d->print("m ");
      float outTemp = props->deps->outsideTemp->getAverage();
      if (!isnan(outTemp)) {
        props->d->print((int)outTemp);
        props->d->print("C");
      }
      props->d->setCursor(2, 38);
      props->d->print(seconds);
      props->d->print("s   ");
      float airQ = props->deps->co2Inside->getAverage();
      if (!isnan(airQ)) {
        props->d->print((int)airQ);
      }
      props->d->display();
    }
    bool canBeDimmed(ScreenProps * deps) {
      return false;
    }
    bool shouldShowScreenSaver(ScreenProps * deps) {
      return false;
    }
    int getDelayMs(ScreenProps * deps) {
      return 250;
    }
    bool handleClick(ScreenProps * deps, byte times){
      return false;
    }

    bool handleHold(ScreenProps * props, int duration_ms, bool finished){
      props->deps->debugger->trace("Boosting..");
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
