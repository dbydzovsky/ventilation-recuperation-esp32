
#include "Arduino.h"
#include "ScreenFactory.h"
#include "LogoScreen.cpp"
#include "MainScreen.cpp"
#include "PressButtonScreen.cpp"
#include "WiFiScreen.cpp"
#include "DisableScreen.cpp"
#include "BoostScreen.cpp"
#include "ManualScreen.cpp"
#include "Co2HistoryScreen.cpp"
#include "TempHistoryScreen.cpp"

ScreenFactory::ScreenFactory() {
	this->logoScreen = new LogoScreen();
	this->mainScreen = new MainScreen();
    this->pressButtonScreen = new PressButtonScreen();
    this->wifiScreen = new WifiScreen();
    this->disableScreen = new DisableScreen();
    this->boostScreen = new BoostScreen();
    this->trialScreen = new ManualScreen();
    this->co2History = new Co2HistoryScreen();
    this->tempHistory = new TempHistoryScreen();
}