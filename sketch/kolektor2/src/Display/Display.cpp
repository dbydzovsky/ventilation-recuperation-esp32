#include "Arduino.h"
#include "Display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "TimeLib.h"
#define OLED_RESET 0
#include "../Dependencies/Dependencies.h"
#include "../Orchestrator/Orchestrator.h"
#include "../DisplayScreen/ScreenFactory.h"

// http://adafruit.github.io/Adafruit-GFX-Library/html/class_adafruit___g_f_x.html
// https://diyusthad.com/image2cpp



bool Display::shouldBeDimmed() {
  return this->actual->canBeDimmed(this->screenProps) && millis() - this->last_interaction > KEEP_DISPLAY_BRIGHT_FOR;
}

bool Display::shouldShowScreenSaver() {
  return this->actual->shouldShowScreenSaver(this->screenProps) && millis() - this->last_interaction > SHOW_SCREEN_SAVER_AFTER;
}

bool Display::handleHold(int duration_ms, bool finished) {
	if (!this->actual->hasActiveButton()) {
	  return false;
	}
	if (IS_DEBUG) Serial.print("handleHold,duration: ");
	if (IS_DEBUG) Serial.print(duration_ms);
	if (IS_DEBUG) Serial.print(" ,finished");
	if (IS_DEBUG) Serial.println(finished);
	this->last_interaction = millis();
	if (!this->btnPressDone) {
		this->isButtonPress = !finished;
		if (this->screenFactory->pressButtonScreen->isFinished(this->screenProps)) {
			this->btnPressDone = true;
			this->isButtonPress = false;
			this->actual->handleHold(this->screenProps, duration_ms, finished);
			this->screenIndex = 0;
			this->actual = this->getDefaultScreen();
			this->actual->setup(this->screenProps);
			this->actual->tick(this->screenProps);
			this->last_tick = millis();
		}
	}
	return false;
}

Screen* Display::getDefaultScreen() {
  if (this->deps->factory->Trial->isValid(this->deps->conf->getData())) {
	return this->screenFactory->trialScreen;
  }
  return this->screenFactory->mainScreen;
}

#define SCREEN_COUNT 8

Screen* Display::getActualScreen() {
  if (this->deps->factory->Trial->isValid(this->deps->conf->getData())) {
	return this->screenFactory->trialScreen;
  }
  if (this->screenIndex == 1) {
    return this->screenFactory->co2History;
  } else if (this->screenIndex == 2) {
    return this->screenFactory->tempHistory;
  } else if (this->screenIndex == 3) {
    return this->screenFactory->disableScreen;
  } else if (this->screenIndex == 4) {
	return this->screenFactory->boostScreen;
  } else if (this->screenIndex == 5) {
	return this->screenFactory->inHouseScreen;
  } else if (this->screenIndex == 6) {
	return this->screenFactory->outHouseScreen;
  } else if (this->screenIndex == 7) {
	return this->screenFactory->debugScreen;
  } else {
	return this->getDefaultScreen();
  }
}

bool Display::handleClick(byte times) {
	if (IS_DEBUG) Serial.print("handleClick");
	if (IS_DEBUG) Serial.println(times);
	this->last_interaction = millis();
	if (this->_wokeUpFromScreenSaver) {
	  this->_wokeUpFromScreenSaver = false;
	} else if (times == 1) {
      this->screenIndex = (this->screenIndex + 1) % SCREEN_COUNT;  
	} else if (times == 2) {
	  this->screenIndex = (this->screenIndex - 1) % SCREEN_COUNT;
	} else {
	  this->actual->handleClick(this->screenProps, times);
	  return true;
	}
	this->actual = this->getActualScreen();
	this->actual->setup(this->screenProps);
	this->last_tick = millis();
	this->actual->tick(this->screenProps);
	return true;
}

void Display::setPass(long pass) {
	this->screenFactory->debugScreen->setPass(pass);
}
void Display::onPressDown() {
	bool showScreenSaver = this->shouldShowScreenSaver();
	if (showScreenSaver) {
	  this->_wokeUpFromScreenSaver = true;
	}
	this->last_interaction = millis();
	if (!this->actual->hasActiveButton()) {
	  return;
	}
	if (!showScreenSaver) {
	  if (IS_DEBUG) Serial.println("onPressDown");
	  this->btnPressDone = false;
	  this->screenFactory->pressButtonScreen->setup(this->screenProps);
	  this->actual->onPressDown(this->screenProps);	
	}
}

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

Display::Display(Dependencies * deps, Orchestrator * orchestrator){
  Adafruit_SSD1306 * display = new Adafruit_SSD1306(OLED_RESET);
  this->d = display;
  this->deps = deps;
  this->orchestrator = orchestrator;
  this->screenFactory = new ScreenFactory();
  this->actual = this->screenFactory->logoScreen;
  this->screenProps = new ScreenProps();
  screenProps->d = display;
  screenProps->deps = deps;
  screenProps->orchestrator = orchestrator;
}

void Display::wifiConnecting() {
  this->actual = this->screenFactory->wifiScreen;
  this->actual->setup(this->screenProps);
}

void Display::setup() {
  this->d->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  this->d->clearDisplay();
  this->d->display();
  this->actual->setup(this->screenProps);
  this->last_tick = millis();
  this->ticker.attach_ms(100, &Display::staticTickerCallbackLed, this);
}

void Display::staticTickerCallbackLed(Display *pThis){
    pThis->tick();
}

void Display::tick() {
  if (!this->initialized) {
	if (millis() - this->last_tick < this->actual->getDelayMs(this->screenProps)) {
	  return;
	}
	this->last_tick = millis();
	this->actual->tick(this->screenProps);
  }
}

void Display::act(){ 
  if (!this->initialized) {
	this->initialized = true;
  	this->ticker.detach();
  	delay(200);
  	this->d->begin(SSD1306_SWITCHCAPVCC, 0x3C);
	this->actual->finish();
  }
  if (this->_reinitScreen) {
	this->d->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  }
  if (this->shouldBeDimmed()) {
	  this->d->dim(true);
  } else {
	  this->d->dim(false);
  }
  if (millis() - this->last_sync > 120000) {
	this->_historyUpdateIndex = (this->_historyUpdateIndex + 1) % 30;
	if (this->_historyUpdateIndex == 0) {
		this->d->begin(SSD1306_SWITCHCAPVCC, 0x3C);
	}
	this->last_sync = millis();
	this->screenFactory->co2History->updateHistory(this->screenProps);
	this->screenFactory->tempHistory->updateHistory(this->screenProps);
  }
  if (this->isButtonPress) {
	if (millis() - this->last_tick > this->screenFactory->pressButtonScreen->getDelayMs(this->screenProps)) {
      if (IS_DEBUG) Serial.println("ticking button");
	  this->screenFactory->pressButtonScreen->tick(this->screenProps);
	}
	return;
  }
  bool showScreenSaver = this->shouldShowScreenSaver();
  if (this->actual->isFinished(this->screenProps)) {
	this->screenIndex = 0;
	this->actual = this->getDefaultScreen();
	this->actual->setup(this->screenProps);
	this->actual->tick(this->screenProps);
	this->last_tick = millis();
  } else {
	// if not finished, check its time
	if (showScreenSaver) {
		if (millis() - this->last_tick < SCREEN_SAVER_TICK_DURATION) {
			return;
		}
	} else {
		if (millis() - this->last_tick < this->actual->getDelayMs(this->screenProps)) {
			return;
		}
	}
  }

  Screen * newOne = this->getActualScreen();
  if (newOne != this->actual) {
	this->actual = newOne;
	this->actual->setup(this->screenProps);
	this->last_interaction = millis();
  }
  this->last_tick = millis();
  if (showScreenSaver) {
	this->d->clearDisplay();
	// todo screensaver
	// this->d->setCursor(0, 0);
	// this->d->setTextSize(2);
	// this->d->setTextColor(WHITE);
	// this->d->print("SAVER");
	this->d->display();
  } else {
	this->actual->tick(this->screenProps);
  }
}