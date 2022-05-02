#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenFactory.h"
#include "TimeLib.h"
#define OLED_RESET 0
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"


class RestartingScreen: public Screen {
  private:
    int index;
    bool finished;
  public:
    void setup(ScreenProps * props){
      this->index = props->d->height(); 
      this->finished = false;
    }
    bool isFinished(ScreenProps * deps) {
      return this->finished;
    }
    void finish() {
      this->finished = true;
    };
    void tick(ScreenProps * props){
      int height = props->d->height();
      int width = props->d->width();
      if (this->index < 0) {
        this->index = props->d->height();
        this->finished = true;
      }
      props->d->clearDisplay();
      if (this->finished) {
        props->d->setCursor(0,20);
        props->d->setTextSize(1);
        props->d->setTextColor(WHITE);
        props->d->print("Restarting");
      }      
      int third = (height / 3);
      props->d->drawCircle(width/2, height/2, this->index, WHITE);
      props->d->drawCircle(width/2, height/2, (this->index + third) % height, WHITE);
      props->d->drawCircle(width/2, height/2, (this->index + (third * 2)) % height, WHITE);
      props->d->display();
      this->index -= 2;
    }
    bool canBeDimmed(ScreenProps * deps) {
      return false;
    }
    bool shouldShowScreenSaver(ScreenProps * deps) {
      return false;
    }
    int getDelayMs(ScreenProps * deps) {
      return 50;
    }
    bool handleClick(ScreenProps * deps, byte times){
      return false;
    }
    bool handleHold(ScreenProps * deps, int duration_ms, bool finished){
      return false;
    }
    void onPressDown(ScreenProps * deps) {

    }

    bool hasActiveButton() {
      return false;
    }
};
