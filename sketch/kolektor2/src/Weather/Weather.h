/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef Weather_h
#define Weather_h
#include "Arduino.h"
#include "HTTPClient.h"

#include "../TimeProvider/TimeProvider.h"
#include "../Configuration/Configuration.h"
#include "../Lock/Lock.h"

struct WeatherDeps {
  ConfigurationData * data;
  TimeProvider * timeProvider;
  HTTPClient * httpClient;
  Lock * httpsLock;
};

class WeatherForecast {
  public:
    WeatherForecast();
    bool hasValidForecast();
    void act(WeatherDeps * deps);
    bool shouldCoolInsides();
  private:
    bool sync(WeatherDeps * deps);
    float feelsLikeToday = 0;
    short lastStatusCode = -100;
    unsigned long last_retrival;
    unsigned long last_success;
};

#endif