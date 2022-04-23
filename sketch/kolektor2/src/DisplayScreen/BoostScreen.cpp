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

static const unsigned char PROGMEM dring_icon16x16[] =
{
	0b00110000, 0b00001100, 
	0b00110000, 0b00001100, 
	0b00111111, 0b11111100, 
	0b00111111, 0b11111100, 
	0b00111111, 0b11111100, 
	0b00011111, 0b11111000, 
	0b00011111, 0b11111000, 
	0b00000111, 0b11100000, 
	0b00000001, 0b10000000, 
	0b00000001, 0b10000000, 
	0b00000001, 0b10000000, 
	0b00000001, 0b10000000, 
	0b00000011, 0b11000000, 
	0b00000011, 0b11000000, 
	0b00111111, 0b11111100, 
	0b00000000, 0b00000000, 
};

class BoostScreen: public Screen {
  private:
	unsigned long opened_since = millis(); 
  public:
    void setup(ScreenProps * deps){
	    this->opened_since = millis();
    }
    bool isFinished(ScreenProps * deps) {
      return millis() - this->opened_since > KEEP_SCREEN_SHORT;
    }
    void finish() {
      
    };
    void tick(ScreenProps * props){ 
      props->d->clearDisplay();
      props->d->drawBitmap(24, 32, dring_icon16x16, 16, 16, WHITE);
      props->d->setTextSize(2);
      props->d->setTextColor(WHITE);
      props->d->setCursor(2,10);
      props->d->print("PARTY");
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
      if (IS_DEBUG) Serial.println("Boosting..");
      ConfigurableProgramme * trialProgramme = props->deps->factory->Trial;
      PowerOutput output;
      SettingsData * settings = props->deps->settings->getSettings();
      output.mode = POWER_OUTPUT_MODE_BOTH;
      output.ventilatorPower = settings->ventilatorPower;    
      output.recuperationPower = settings->recuperationPower;
      output.recuperationMode = settings->recuperationMode;
      trialProgramme->setPower(output, settings->durationMillis);
      props->deps->debugger->debug("Starting Trial programme (BOOST)");
      props->orchestrator->setProgramme(dynamic_cast<Programme*>(trialProgramme));
      return true;
    }

    void onPressDown(ScreenProps * deps) {

    }
    bool hasActiveButton() {
      return true;
    }
};
