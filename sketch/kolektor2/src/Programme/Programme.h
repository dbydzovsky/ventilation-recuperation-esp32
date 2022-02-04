#ifndef Programme_h
#define Programme_h
#include "Arduino.h"
#include "../Configuration/Configuration.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Constants/Constants.h"
#define temperatureDownTolerationProgramme 2
#define warmingUpTime 40000
#include "../Weather/Weather.h"


typedef struct {
  ConfigurationData* data;
  float tempInside;
  float tempOutside;
  float humidityOutside;
  float dewPoint;
  WeatherForecast * forecast;
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
    virtual byte getCode() = 0;
};

class ConfigurableProgramme: public Programme {
  public:
    virtual void setPower(PowerOutput output, int duration) = 0;
};

class ProgrammeFactory {
  public:
    ProgrammeFactory();
    Programme *Disabled;
    Programme *Summer;
    Programme *Winter;
    Programme *Initial;
    Programme *Error;
    Programme *Auto;
    ConfigurableProgramme *Trial;
};
#endif