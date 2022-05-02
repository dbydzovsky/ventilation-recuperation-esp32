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

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 48
#define BALL_R 3
class ScreenSaverScreen: public Screen {
  private:
    int16_t x = SCREEN_WIDTH / 2;
    int16_t y = SCREEN_HEIGHT / 2;
    int xspeed = 2;
    int yspeed = 2;
  public:
    void setup(ScreenProps * deps){
      this->y = SCREEN_HEIGHT / 2;
      this->x = SCREEN_WIDTH / 2;
      deps->deps->debugger->trace("Screen saver setup");
    }
    bool isFinished(ScreenProps * deps) {
      return false;
    }
    void finish() {
      
    };
    void tick(ScreenProps * props){ 
	    props->d->clearDisplay();
      this->x = this->x + this->xspeed;
      this->y = this->y + this->yspeed;
      if (this->x > (SCREEN_WIDTH - BALL_R) || this->x < (0 + BALL_R)) {
        this->xspeed = this->xspeed * -1;
      }
      if (this->y > (SCREEN_HEIGHT - BALL_R) || this->y < (0 + BALL_R)) {
        this->yspeed = this->yspeed * -1;
      }
      // drawCircle / fillCircle
      props->d->drawCircle(this->x, this->y, BALL_R, WHITE);
      props->d->display();
    }
    bool canBeDimmed(ScreenProps * deps) {
      return true;
    }
    bool shouldShowScreenSaver(ScreenProps * deps) {
      return true;
    }
    int getDelayMs(ScreenProps * deps) {
      return 200;
    }
    bool handleClick(ScreenProps * deps, byte times){
      return false;
    }
    bool handleHold(ScreenProps * props, int duration_ms, bool finished){
      return false;
    }

    void onPressDown(ScreenProps * deps) {

    }

    bool hasActiveButton() {
      return false;
    }
};
