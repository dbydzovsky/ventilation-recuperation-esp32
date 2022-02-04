#include "Arduino.h"
#include "Weather.h"

WeatherForecast::WeatherForecast() {

}

bool WeatherForecast::hasValidForecast(){
  return false;
}
void WeatherForecast::act() {

}
bool WeatherForecast::shouldCoolInsides(){
  return true;
}
