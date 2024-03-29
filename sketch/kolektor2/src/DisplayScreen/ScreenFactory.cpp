
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
#include "InHouseScreen.cpp"
#include "DebugScreen.cpp"
#include "OutHouseScreen.cpp"
#include "ScreenSaverScreen.cpp"
#include "RestartingScreen.cpp"

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
    this->inHouseScreen = new InHouseScreen();
    this->debugScreen = new DebugScreen();
    this->outHouseScreen = new OutHouseScreen();
    this->screenSaverScreen = new ScreenSaverScreen();
    this->restartingScreen = new RestartingScreen();
}