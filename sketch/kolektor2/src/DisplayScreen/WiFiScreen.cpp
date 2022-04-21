#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenFactory.h"
#include "TimeLib.h"
#define OLED_RESET 0
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"



static const unsigned char PROGMEM wifi1_icon16x16[] =
{
	0b00000000, 0b00000000, //                 
	0b00000111, 0b11100000, //      ######     
	0b00011111, 0b11111000, //    ##########   
	0b00111111, 0b11111100, //   ############  
	0b01110000, 0b00001110, //  ###        ### 
	0b01100111, 0b11100110, //  ##  ######  ## 
	0b00001111, 0b11110000, //     ########    
	0b00011000, 0b00011000, //    ##      ##   
	0b00000011, 0b11000000, //       ####      
	0b00000111, 0b11100000, //      ######     
	0b00000100, 0b00100000, //      #    #     
	0b00000001, 0b10000000, //        ##       
	0b00000001, 0b10000000, //        ##       
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
};

static const unsigned char PROGMEM wifi2_icon16x16[] =
{
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000111, 0b11100000, //      ######     
	0b00001111, 0b11110000, //     ########    
	0b00011000, 0b00011000, //    ##      ##   
	0b00000011, 0b11000000, //       ####      
	0b00000111, 0b11100000, //      ######     
	0b00000100, 0b00100000, //      #    #     
	0b00000001, 0b10000000, //        ##       
	0b00000001, 0b10000000, //        ##       
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
};
static const unsigned char PROGMEM wifi3_icon16x16[] =
{
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //       
	0b00000000, 0b00000000, //      
	0b00000000, 0b00000000, //     
	0b00000011, 0b11000000, //       ####      
	0b00000111, 0b11100000, //      ######     
	0b00000100, 0b00100000, //      #    #     
	0b00000001, 0b10000000, //        ##       
	0b00000001, 0b10000000, //        ##       
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
};
class WifiScreen: public Screen {
  private:
    int index = 0;
    bool finished = false;
  public:
    void setup(ScreenProps * deps){
      this->index = 0;
      this->finished = false;
    }
    bool isFinished(ScreenProps * deps) {
      return this->finished;
    }
    void finish() {
      this->finished = true;
    };
    void tick(ScreenProps * deps){ 
      if (this->index % 50 == 0) {
        deps->d->begin(SSD1306_SWITCHCAPVCC, 0x3C);
      }
	  deps->d->clearDisplay();
      if (this->index % 3 == 0) {
        deps->d->drawBitmap(24, 16, wifi3_icon16x16, 16, 16, WHITE);
      } else if (this->index % 3 == 1) {
        deps->d->drawBitmap(24, 16, wifi2_icon16x16, 16, 16, WHITE);
      } else {
        deps->d->drawBitmap(24, 16, wifi1_icon16x16, 16, 16, WHITE);
      }
      deps->d->display();
      this->index += 1;
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
    bool handleHold(ScreenProps * deps, int duration_ms, bool finished){
      return false;
    }
    void onPressDown(ScreenProps * deps) {

    }
    bool hasActiveButton() {
      return false;
    }
};
