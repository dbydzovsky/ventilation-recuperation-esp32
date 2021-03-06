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
#include "../HttpServer/HttpServer.h"

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
	  if (!finished && duration_ms > 3000) {
		this->_willRestart = true;
		return false;
	  } else if (duration_ms > 8000) {
		this->deps->restarter->restart();
		return true;
	  } else if (finished) {
		this->_willRestart = false;
		return true;
	  }
	  return false;
	}
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

#define SCREEN_COUNT 9

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
	return this->screenFactory->connectionScreen;
  } else if (this->screenIndex == 8) {
	return this->screenFactory->debugScreen;
  } else {
	return this->getDefaultScreen();
  }
}

bool Display::handleClick(byte times) {
	char buff[50];
	sprintf(buff, "handing click %d", times);
	this->deps->debugger->trace(buff);
	this->last_interaction = millis();
	if (this->_wokeUpFromScreenSaver) {
	  this->_wokeUpFromScreenSaver = false;
	} else if (times == 1) {
      this->screenIndex = (this->screenIndex + 1) % SCREEN_COUNT;  
	} else if (times == 2) {
	  this->screenIndex = (this->screenIndex + SCREEN_COUNT - 1) % SCREEN_COUNT;
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
#define MINIMUM_BETWEEN_REINIT 30000
void Display::onPressDown() {
	bool showScreenSaver = this->shouldShowScreenSaver();
	if (showScreenSaver) {
	  this->_reinitScreen = true;
	  this->_wokeUpFromScreenSaver = true;
	}
	if (millis() - this->last_reinit > MINIMUM_BETWEEN_REINIT) {
	  this->last_reinit = millis();
	  this->_reinitScreen = true;
	}
	this->last_interaction = millis();
	if (!this->actual->hasActiveButton()) {
	  return;
	}
	if (!showScreenSaver) {
	  this->deps->debugger->trace("onPressDown");
	  this->btnPressDone = false;
	  this->screenFactory->pressButtonScreen->setup(this->screenProps);
	  this->actual->onPressDown(this->screenProps);	
	}
}

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

Display::Display(Dependencies * deps, Orchestrator * orchestrator, HttpServer *server){
  Adafruit_SSD1306 * display = new Adafruit_SSD1306(OLED_RESET);
  this->d = display;
  this->deps = deps;
  this->orchestrator = orchestrator;
  this->screenFactory = new ScreenFactory();
  this->actual = this->screenFactory->logoScreen;
  this->screenFactory->debugScreen->setServer(server);
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
	this->d->clearDisplay();
  	this->d->display();
	this->actual->finish();
  }
  if (this->_reinitScreen) {
	this->_reinitScreen = false; 
	this->d->clearDisplay();
  	this->d->display();
	this->d->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  }
  if (this->shouldBeDimmed()) {
	  this->d->dim(true);
  } else {
	  this->d->dim(false);
  }
  if (this->_willRestart) {
	if (!this->_restartingScreenInitialized) {
	  this->_restartingScreenInitialized = true;
	  this->screenFactory->restartingScreen->setup(this->screenProps);
	}
	this->screenFactory->restartingScreen->tick(this->screenProps);
	return;
  } else {
	this->_restartingScreenInitialized = false;
  }
  if (millis() - this->last_sync > 120000) {
	this->_historyUpdateIndex = (this->_historyUpdateIndex + 1) % 10;
	if (this->_historyUpdateIndex == 0) {
		this->d->begin(SSD1306_SWITCHCAPVCC, 0x3C);
		this->d->clearDisplay();
  		this->d->display();
	}
	this->last_sync = millis();
	this->screenFactory->co2History->updateHistory(this->screenProps);
	this->screenFactory->tempHistory->updateHistory(this->screenProps);
  }
  if (this->isButtonPress) {
	if (millis() - this->last_tick > this->screenFactory->pressButtonScreen->getDelayMs(this->screenProps)) {
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
		if (millis() - this->last_tick < this->screenFactory->screenSaverScreen->getDelayMs(this->screenProps)) {
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
	this->screenFactory->screenSaverScreen->tick(this->screenProps);
  } else {
  	this->actual->tick(this->screenProps);
  }
}