
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenFactory.h"
#include "TimeLib.h"
#define OLED_RESET 0
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"
#include "AlarmScreen.cpp"
static const unsigned char PROGMEM heart_icon16x16[] =
{                
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00111100, 0b01111000, //   ####   ####   
	0b01111110, 0b11111100, //  ###### ######  
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b01111111, 0b11111100, //  #############  
	0b01111111, 0b11111100, //  #############  
	0b00111111, 0b11111000, //   ###########   
	0b00011111, 0b11110000, //    #########    
	0b00001111, 0b11100000, //     #######     
	0b00000111, 0b11000000, //      #####      
	0b00000011, 0b10000000, //       ###    
	0b00000001, 0b00000000, //    	  #
};

static const unsigned char PROGMEM nocon_icon16x12[] =
{
	0b00000111, 0b11110000, //      #######    
	0b00011111, 0b11111100, //    ###########  
	0b00111110, 0b00111110, //   #####   ##### 
	0b00111000, 0b01111110, //   ###    ###### 
	0b01110000, 0b11111111, //  ###    ########
	0b01110001, 0b11110111, //  ###   ##### ###
	0b01110011, 0b11000111, //  ###  ####   ###
	0b01110111, 0b10000111, //  ### ####    ###
	0b00111111, 0b00001110, //   ######    ### 
	0b00111110, 0b00011110, //   #####    #### 
	0b00011111, 0b11111100, //    ###########  
	0b00000111, 0b11110000, //      #######    
};

static const unsigned char PROGMEM temperature_icon16x16[] =
{
	0b00000001, 0b11000000, //        ###      
	0b00000011, 0b11100000, //       #####     
	0b00000111, 0b00100000, //      ###  #     
	0b00000111, 0b11100000, //      ######     
	0b00000111, 0b00100000, //      ###  #     
	0b00000111, 0b11100000, //      ######     
	0b00000111, 0b00100000, //      ###  #     
	0b00000111, 0b11100000, //      ######     
	0b00000111, 0b00100000, //      ###  #     
	0b00001111, 0b11110000, //     ########    
	0b00011111, 0b11111000, //    ##########   
	0b00011111, 0b11111000, //    ##########   
	0b00011111, 0b11111000, //    ##########   
	0b00011111, 0b11111000, //    ##########   
	0b00001111, 0b11110000, //     ########    
	0b00000111, 0b11100000, //      ######     
};

static const unsigned char PROGMEM wifi2_icon12x10[] = {
	0b00011111, 0b10000000, //    ######       
	0b00111111, 0b11000000, //   ########      
	0b01100000, 0b01100000, //  ##      ##     
	0b00001111, 0b00000000, //     ####        
	0b00011111, 0b10000000, //    ######       
	0b00010000, 0b10000000, //    #    #       
	0b00000110, 0b00000000, //      ##         
	0b00000110, 0b00000000, //      ##         
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //
};

#define MAX_INDEX 10
class MainScreen: public Screen {
  private:
    int index = 0;
    AlarmScreen * alarmScreen = new AlarmScreen();

  public:
    MainScreen() {
      this->alarmScreen = new AlarmScreen();
    }
    void setup(ScreenProps * props){
      this->index = 0;
      this->alarmScreen->setup(props);
      if (IS_DEBUG) Serial.println("Main screen setup");
    }
    bool isFinished(ScreenProps * deps) {
      return false;
    }
    void finish() {

    }
    void tick(ScreenProps * props){
      this->index = (this->index +1) % MAX_INDEX;
      if (this->index > 5) {
        this->alarmScreen->tick(props);
        return;
      }
      props->d->clearDisplay();
      if (WiFi.status() == WL_CONNECTED) {
        props->d->drawBitmap(52, 0, wifi2_icon12x10, 12, 10, WHITE);
      } else {  
        props->d->drawBitmap(52, 0, nocon_icon16x12, 12, 10, WHITE);
      }

      props->d->setTextSize(1);
      props->d->setTextColor(WHITE);
      
      if (props->deps->timeProvider->isTimeSet()) {
        char hourBuffer[3];
        if (hour() < 10) {
          sprintf(hourBuffer, "0%d", hour());
        } else {
          sprintf(hourBuffer, "%d", hour());
        }
        char minuteBuffer[3];
        if (minute() < 10) {
          sprintf(minuteBuffer, "0%d", minute());
        } else {
          sprintf(minuteBuffer, "%d", minute());
        }
        char timeBuffer[6];
        if (this->index % 2 == 1) {
          sprintf(timeBuffer, "%s:%s", hourBuffer, minuteBuffer);
        } else {
          sprintf(timeBuffer, "%s %s", hourBuffer, minuteBuffer);
        }
        props->d->setCursor(1,0);
        props->d->print(timeBuffer);
      }
      props->d->setTextSize(2);
      props->d->drawBitmap(0,12, temperature_icon16x16, 16, 12, WHITE);
      props->d->setCursor(18,12);
      if (props->deps->outsideTemp->isInitialized()) {  
        props->d->print((int) props->deps->outsideTemp->getAverage());
      } else {
        props->d->print("..");
      }
      props->d->drawBitmap(0,30, heart_icon16x16, 16,16, WHITE);
      props->d->setCursor(16, 30);
      // this->d->drawLine(18, 46, 62, 46, WHITE); // x, height, width
      if (props->deps->co2Inside->isInitialized()) {
        props->d->print((int) props->deps->co2Inside->getAverage());
      } else {
        props->d->print("....");
      }
      // this->d->setTextColor(BLACK, WHITE); // 'inverted' text
      // this->d->print("hum:");
      // this->d->println(hum);
      // this->d->setTextSize(1);
      // this->d->setTextColor(WHITE);
      // this->d->print("co2:");
      // this->d->println(ppm);
      // this->d->display();
      props->d->display();
    }

    bool canBeDimmed(ScreenProps * deps) {
      return true;
    }
    int getDelayMs(ScreenProps * deps) {
      return 1000;
    }
    bool handleClick(ScreenProps * deps, byte times) {
      // todo
      return true;
    }
    bool handleHold(ScreenProps * deps, int duration_ms, bool finished) {
      // todo
      return false;
    }
    void onPressDown(ScreenProps * deps) {
      // todo;
    }
    bool hasActiveButton() {
      return false;
    }
};
