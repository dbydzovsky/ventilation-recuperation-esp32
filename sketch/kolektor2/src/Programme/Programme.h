#ifndef Programme_h
#define Programme_h
#include "Arduino.h"
#include "../Configuration/Configuration.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Constants/Constants.h"
#include "../Weather/Weather.h"
#include <WiFiClientSecure.h>

#define temperatureDownTolerationProgramme 2
#define warmingUpTime 40000

typedef struct {
  ConfigurationData* data;
  float tempInside;
  float tempOutside;
  float humidityOutside;
  float co2;
  float dewPoint;
  WeatherForecast * forecast;
  WeatherDeps * weatherDeps;
  bool isTimeSet;
} ProgrammeContext;

struct PowerOutput {
  byte mode = POWER_OUTPUT_MODE_NONE;
  byte ventilatorPower = 0;
  byte recuperationPower = 0;
  byte recuperationMode = RECUPERATION_MODE_RECYCLE;
};

class Programme {
  public:
    virtual int getDescription(char dest[80]) = 0;
    virtual void onStart() = 0;
    virtual bool handleClick(byte times) = 0;
    virtual bool handleHold(int duration_ms, bool finished) = 0;
    virtual void getPower(ProgrammeContext * context, PowerOutput *out) = 0;
    virtual void configureTicking(RGBDiode *diode) = 0;
    virtual bool isValid(ConfigurationData* data) = 0;
    virtual bool canForce() = 0;
    virtual void invalidate() = 0;
    virtual byte getCode() = 0;
};

class ConfigurableProgramme: public Programme {
  public:
    virtual void setPower(PowerOutput output, int duration) = 0;
    virtual PowerOutput getActualSetting() = 0;
    virtual int getDuration() = 0;
};

class ProgrammeFactory {
  public:
    ProgrammeFactory();
    Programme *Disabled;
    Programme *Recuperation;
    Programme *Summer;
    Programme *Winter;
    Programme *Initial;
    Programme *Error;
    Programme *Auto;
    ConfigurableProgramme *Trial;
};
#endif