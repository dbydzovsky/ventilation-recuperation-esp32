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

static const unsigned char PROGMEM powerbutton_icon16x16[] =
{
	0b00000000, 0b00000000, //                 
	0b00000001, 0b11000000, //        ###      
	0b00001101, 0b11011000, //     ## ### ##   
	0b00011101, 0b11011100, //    ### ### ###  
	0b00111101, 0b11011110, //   #### ### #### 
	0b00111001, 0b11001110, //   ###  ###  ### 
	0b01110001, 0b11000111, //  ###   ###   ###
	0b01110001, 0b11000111, //  ###   ###   ###
	0b01110001, 0b11000111, //  ###   ###   ###
	0b01110000, 0b00000111, //  ###         ###
	0b00111000, 0b00001110, //   ###       ### 
	0b00111110, 0b00011110, //   #####    #### 
	0b00011111, 0b11111100, //    ###########  
	0b00001111, 0b11111000, //     #########   
	0b00000011, 0b11100000, //       #####     
	0b00000000, 0b00000000, //                 
};

class DisableScreen: public Screen {
  private:
	unsigned long opened_since = millis(); 
  public:
    void setup(ScreenProps * deps){
	    this->opened_since = millis();
      deps->deps->debugger->trace("Disable screen setup");
    }
    bool isFinished(ScreenProps * deps) {
      return millis() - this->opened_since > KEEP_SCREEN_SHORT;
    }
    void finish() {
      
    };
    void tick(ScreenProps * props){ 
	    props->d->clearDisplay();
      
      props->d->setTextSize(2);
      props->d->setTextColor(WHITE);
      props->d->setCursor(12,0);
      props->d->print("TEMP");
      props->d->setCursor(16,16);
      props->d->print("OFF");
      props->d->drawBitmap(24, 32, powerbutton_icon16x16, 16, 16, WHITE);
      props->d->display();
    }
    bool canBeDimmed(ScreenProps * deps) {
      return false;
    }
    bool shouldShowScreenSaver(ScreenProps * deps) {
      return true;
    }
    int getDelayMs(ScreenProps * deps) {
      return 500;
    }
    bool handleClick(ScreenProps * deps, byte times){
      return false;
    }

    bool handleHold(ScreenProps * props, int duration_ms, bool finished){
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
