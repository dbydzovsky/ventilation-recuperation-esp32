/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Display_h
#define Display_h
#include "Arduino.h"

#include <Ticker.h>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"
#include "../DisplayScreen/ScreenFactory.h"

#define KEEP_DISPLAY_BRIGHT_FOR 6000 // a minute

class Display {
  public:
    Display(Dependencies * deps, Orchestrator *orchestrator);
    void setup();
    void act();
    bool handleHold(int durationMillis, bool finished);
    bool handleClick(byte times);
    void onPressDown();
    void wifiConnecting();
  private:
    int screenIndex = 0;
    bool isButtonPress = false;
    bool btnPressDone = false;
    Screen * getActualScreen();
    Screen * getDefaultScreen();
    Screen * actual;
    ScreenFactory * screenFactory;
    Ticker ticker;
    ScreenProps * screenProps;
    Orchestrator * orchestrator;
    Adafruit_SSD1306 * d;
    Dependencies * deps;

    unsigned long last_interaction = millis();
    unsigned long last_tick;
    unsigned long last_sync = millis();
    static void staticTickerCallbackLed(Display *pThis);
    void tick();
    bool initialized = false;
    bool shouldBeDimmed();
};


#endif