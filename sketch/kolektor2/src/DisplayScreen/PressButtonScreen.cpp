#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenFactory.h"
#include "TimeLib.h"
#define OLED_RESET 0
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"

static const unsigned char PROGMEM check_icon16x16[] =
{
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000111, //              ###
	0b00000000, 0b00001111, //             ####
	0b00000000, 0b00011111, //            #####
	0b01110000, 0b00111110, //  ###      ##### 
	0b01111000, 0b01111100, //  ####    #####  
	0b01111100, 0b11111000, //  #####  #####   
	0b00011111, 0b11110000, //    #########    
	0b00001111, 0b11100000, //     #######     
	0b00000111, 0b11000000, //      #####      
	0b00000011, 0b10000000, //       ###       
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
};

class PressButtonScreen: public Screen {
  private:
    int index = 0;
    bool finished = false;
  public:
    void setup(ScreenProps * props){
      this->index = 0; 
      this->finished = false;
    }
    bool isFinished(ScreenProps * deps) {
      return this->finished;
    }
    void finish() {
      this->finished = true;
    };
    void tick(ScreenProps * props){
      if (this->finished) {
        props->d->clearDisplay();
        props->d->drawBitmap(24, 16, check_icon16x16, 16, 16, WHITE);
        props->d->display();
        return;
      }
      int height = props->d->height();
      int width = props->d->width();
      props->d->clearDisplay();
      props->d->drawCircle(width/2, height/2, this->index, WHITE);
      props->d->display();
      if (this->index > height) {
        this->finished = true;
      }
      this->index += 2;
    }
    bool canBeDimmed(ScreenProps * deps) {
      return false;
    }
    int getDelayMs(ScreenProps * deps) {
      return 10;
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
