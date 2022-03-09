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

static const unsigned char PROGMEM warning_icon16x16[] =
{
	0b00000000, 0b10000000, //         #       
	0b00000001, 0b11000000, //        ###      
	0b00000001, 0b11000000, //        ###      
	0b00000011, 0b11100000, //       #####     
	0b00000011, 0b01100000, //       ## ##     
	0b00000111, 0b01110000, //      ### ###    
	0b00000110, 0b00110000, //      ##   ##    
	0b00001110, 0b10111000, //     ### # ###   
	0b00001100, 0b10011000, //     ##  #  ##   
	0b00011100, 0b10011100, //    ###  #  ###  
	0b00011000, 0b10001100, //    ##   #   ##  
	0b00111000, 0b00001110, //   ###       ### 
	0b00110000, 0b10000110, //   ##    #    ## 
	0b01111111, 0b11111111, //  ###############
	0b01111111, 0b11111111, //  ###############
	0b00000000, 0b00000000, //                 
};


class AlarmScreen: public Screen {
  private:
    const char text[12] = "";
  public:
    void setup(ScreenProps * deps){
	    if (IS_DEBUG) Serial.println("Alarm screen setup");
    }
    bool isFinished(ScreenProps * deps) {
      return false;
    }
    void finish() {
      
    };
    void tick(ScreenProps * props){ 
      props->d->clearDisplay();
      props->d->drawBitmap(0, 0, warning_icon16x16, 16, 16, WHITE);
      props->d->drawBitmap(48, 0, warning_icon16x16, 16, 16, WHITE);
      props->d->setTextSize(1);
      props->d->setTextColor(WHITE);
      props->d->setCursor(18,2);
      props->d->print("ALARM");
      props->d->setCursor(0,20);
      props->d->print(" Nevalidni ");
      props->d->setCursor(0,30);
      props->d->print("  Hodnota  ");
      props->d->setCursor(0,40);
      props->d->print("   cidla   ");
      props->d->display();
    }
    bool canBeDimmed(ScreenProps * deps) {
      return false;
    }
    int getDelayMs(ScreenProps * deps) {
      return 500;
    }
    bool handleClick(ScreenProps * deps, byte times){
      return false;
    }

    bool handleHold(ScreenProps * props, int duration_ms, bool finished){
      if (IS_DEBUG) Serial.println("Boosting..");
      ConfigurableProgramme * trialProgramme = props->deps->factory->Trial;
      PowerOutput output;
      SettingsData * settings = props->deps->settings->getSettings();
      output.mode = POWER_OUTPUT_MODE_BOTH;
      output.ventilatorPower = settings->ventilatorPower;    
      output.recuperationPower = settings->recuperationPower;
      output.recuperationMode = settings->recuperationMode;
      trialProgramme->setPower(output, settings->tempDisableDuration);
      props->orchestrator->setProgramme(dynamic_cast<Programme*>(trialProgramme));
      return true;
    }

    void onPressDown(ScreenProps * deps) {

    }
    bool hasActiveButton() {
      return true;
    }
};
