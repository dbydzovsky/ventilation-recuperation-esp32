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


class TempHistoryScreen: public HistoryScreen {
  private:
    int _min = 1280;
    int _max = -1280;
    int values[63] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    byte index = 0;
    unsigned long opened_since;
  public:
    void setup(ScreenProps * deps){
	    this->opened_since = millis();
    }
    bool isFinished(ScreenProps * deps) {
      return millis() - this->opened_since > KEEP_SCREEN_LONG;
    }
    void finish() {
      
    };
    void updateHistory(ScreenProps * props){
      this->index = (this->index + 1) % 63;
      float tempValue = props->deps->outsideTemp->getAverage();
      if (!isnan(tempValue)) {
        this->values[this->index] = tempValue *10;
        this->_min = tempValue * 10;
        this->_max = tempValue * 10;
        for (int i = 0; i < 63; i++) {
          int iValue = this->values[i];
          if (iValue > this->_max) {
            this->_max = iValue;
          } else if (iValue < this->_min) {
            this->_min = iValue;
          }
        } 
      } else {
        this->values[this->index] = this->_min;
      }
    }
    void tick(ScreenProps * props){ 
      props->d->clearDisplay();
      props->d->setTextSize(1);
      props->d->setTextColor(WHITE);
      props->d->setCursor(0,0);
      int minimum = min(this->_min, this->_max);
      int maximum = max(this->_min, this->_max);
      props->d->print("Teplota");
      props->d->setCursor(50, 0);
      props->d->print("2h");
      props->d->drawLine(0, 10, 0, 47, WHITE);
      props->d->drawLine(0, 47, 64, 47, WHITE);
      for (int i = 0; i < 63; i++) {
        int currentIndex = (this->index + i) % 63;
        int nextIndex = (currentIndex + 1) % 63;
        int value = map(this->values[currentIndex], minimum, maximum, 47, 10);
        if (i == 62) {
          props->d->drawPixel(i, value, WHITE);
        } else {
          int nextValue = map(this->values[nextIndex], minimum, maximum, 47, 10);
          props->d->drawLine(i, value, i, nextValue, WHITE);
        }  
      }
      props->d->setTextColor(WHITE, BLACK);
      props->d->setCursor(2, 10); //x,y
      props->d->print(maximum/10);
      props->d->setCursor(2, 38); //x,y
      props->d->print(minimum/10);
      props->d->display();
    }
    bool canBeDimmed(ScreenProps * deps) {
      return false;
    }
    int getDelayMs(ScreenProps * deps) {
      return 30000;
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
