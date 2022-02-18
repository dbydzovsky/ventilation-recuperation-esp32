/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Dependencies_h
#define Dependencies_h
#include "Arduino.h"
#include <HTTPClient.h>

#include "../Ventilator/Ventilator.h"
#include "../Recuperation/Recuperation.h"
#include "../Lock/Lock.h"
#include "../Programme/Programme.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Configuration/Configuration.h"
#include "../Average/Average.h"
#include "../Weather/Weather.h"
#include "../TimeProvider/TimeProvider.h"
#include "../DewPoint/DewPoint.h"
#include "../RPMChecker/RPMChecker.h"
#include "../Settings/Settings.h"
#include "../Restarter/Restarter.h"

struct Dependencies {
  Ventilator * ventilation;
  Recuperation * recuperation;
  Lock * confLock;
  Lock * httpLock;
  ProgrammeFactory * factory;
  RGBDiode * diode;
  Configuration * conf;
  Average * outsideTemp;
  Average * outsideHum;
  Average * insideTemp;
  Average * co2Inside;
  DewPoint * dewPoint;
  WeatherForecast * forecast;
  TimeProvider * timeProvider;
  HTTPClient * httpClient;
  RPMChecker * ventilatorChecker;
  RPMChecker * recuperationChecker;
  Settings * settings;
  Restarter * restarter;
};

#endif