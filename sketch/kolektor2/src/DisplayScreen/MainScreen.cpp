
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

static const unsigned char PROGMEM nocon_icon12x10[] =
{
	0b00001111, 0b10000000, //      #####    
	0b00111000, 0b11100000, //    ###   ###  
	0b01110001, 0b11110000, //   ###   ##### 
	0b01100011, 0b11110000, //  ###   #######
	0b01100111, 0b00110000, //  ###  ###  ###
	0b01101110, 0b00110000, //  ### ###   ###
	0b01111100, 0b01110000, //   #####   ### 
	0b00111000, 0b11100000, //    ###   ###  
	0b00001111, 0b10000000, //      ##### 
	0b00000000, 0b00000000, //
};


static const unsigned char PROGMEM warning_icon12x10[] =
{
	0b00000010, 0b00000000, //        #      
	0b00000101, 0b00000000, //       # #     
	0b00001101, 0b10000000, //      ## ##    
	0b00001000, 0b10000000, //      #   #    
	0b00010010, 0b01000000, //     #  #  #  
	0b00110010, 0b01100000, //    ##  #  ##  
	0b01100000, 0b00110000, //   ##       ## 
	0b01100010, 0b00110000, //   ##   #   ## 
	0b01111111, 0b11110000, //   ########### 
	0b00000000, 0b00000000, //
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

#define MAX_INDEX 15
class MainScreen: public Screen {
  private:
    int index = 0;
    AlarmScreen * alarmScreen;

  public:
    MainScreen() {
      this->alarmScreen = new AlarmScreen();
    }
    void setup(ScreenProps * props){
      this->index = 0;
      this->alarmScreen->setup(props);
      props->deps->debugger->trace("Main screen setup");
    }
    bool isFinished(ScreenProps * deps) {
      return false;
    }
    void finish() {

    }
    bool hasError(ScreenProps * props) {
        FilterReport ventReport;
        props->deps->filter->report(FAN_TYPE_VENTILATOR, &ventReport);
        FilterReport recReport;
        props->deps->filter->report(FAN_TYPE_RECUPERATION, &recReport);
        if (props->deps->recuperationChecker->shouldStop()) {
          this->alarmScreen->setText("", "   Chyba   ", "rekuperace");
        } else if (props->deps->ventilatorChecker->shouldStop()) {
          this->alarmScreen->setText("", "   Chyba   ", "ventilatoru");
        } else if (!props->deps->outsideTemp->isInitialized()) {
          this->alarmScreen->setText("  Venkovni ", "   cidlo   ", "  teploty  ");
        } else if (!props->deps->outsideHum->isInitialized()) {
          this->alarmScreen->setText("  Venkovni ", "   cidlo   ", "  vlhkosti ");
        } else if (!props->deps->co2Inside->isInitialized()) {
          this->alarmScreen->setText("", "   Cidlo   ", "    CO2");
        } else if (!props->deps->insideTemp->isInitialized()) {
          this->alarmScreen->setText("  Vnitrni", "   cidlo   ", "  teploty  ");
        } else if (!props->deps->insideHum->isInitialized()) {
          this->alarmScreen->setText("  Vnitrni", "   cidlo   ", "  vlhkosti ");
        } else if (recReport.needCleaning) {
          this->alarmScreen->setText("  Vycistit ", "   FILTR   ", "rekuperace");
        } else if (ventReport.needCleaning) {
          this->alarmScreen->setText("  Vycistit ", "   FILTR   ", " ventilace");
        } else {
          return false;
        }
        return true;
    }

    void tick(ScreenProps * props){
      this->index = (this->index +1) % MAX_INDEX;
      bool hasError = this->hasError(props);
      if (this->index > 5) {
        if (hasError) {
          this->alarmScreen->tick(props);
          return;
        }
      }
      props->d->clearDisplay();
      if (WiFi.status() == WL_CONNECTED) {
        props->d->drawBitmap(52, 0, wifi2_icon12x10, 12, 10, WHITE);
      } else {  
        props->d->drawBitmap(52, 0, nocon_icon12x10, 12, 10, WHITE);
      }
      if (hasError) {
        props->d->drawBitmap(40, 0, warning_icon12x10, 12, 10, WHITE);
      }

      props->d->setTextSize(1);
      props->d->setTextColor(WHITE);
      if (this->index > 7 || !props->deps->forecast->hasValidForecast()) {
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
        } else {
          props->d->setCursor(1,0);
          props->d->print("notime");
        }
      } else {
        props->d->setCursor(1,0);
        props->d->print((int) props->deps->forecast->howDoesItFeelLike());
        props->d->print(" C");
      }
      
      props->d->setTextSize(2);
      props->d->drawBitmap(0,12, temperature_icon16x16, 16, 16, WHITE);
      props->d->setCursor(16,12);
      if (props->deps->outsideTemp->isInitialized()) {  
        props->d->print((int) props->deps->outsideTemp->getAverage());
      } else {
        props->d->print("..");
      }
      props->d->drawBitmap(0,30, heart_icon16x16, 16,16, WHITE);
      props->d->setCursor(16, 32);
      // this->d->drawLine(18, 46, 62, 46, WHITE); // x, height, width
      if (props->deps->co2Inside->isInitialized()) {
        props->d->print((int) props->deps->co2Inside->getAverage());
      } else {
        props->d->print("....");
      }
      props->d->display();
    }

    bool canBeDimmed(ScreenProps * deps) {
      return true;
    }
    bool shouldShowScreenSaver(ScreenProps * deps) {
      if (deps->deps->ventilation->getIntendedPower() != 0) {
        return false;
      }
      if (deps->deps->recuperation->getPower() != 0) {
        return false;
      }
      return !this->hasError(deps);
    }
    int getDelayMs(ScreenProps * deps) {
      return 1000;
    }
    bool handleClick(ScreenProps * deps, byte times) {
      return true;
    }
    bool handleHold(ScreenProps * props, int duration_ms, bool finished) {
      ConfigurableProgramme * trialProgramme = props->deps->factory->Trial;
      PowerOutput output;
      SettingsData * settings = props->deps->settings->getSettings();
      trialProgramme->setPower(output, settings->tempDisableDuration);
      props->deps->debugger->debug("Starting Trial programme (Disabled)");
      props->orchestrator->setProgramme(dynamic_cast<Programme*>(trialProgramme));
      return true;
    }
    void onPressDown(ScreenProps * deps) {
    }
    bool hasActiveButton() {
      return true;
    }
};
