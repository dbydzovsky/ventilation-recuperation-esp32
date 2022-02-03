#ifndef Programme_h
#define Programme_h
#include "Arduino.h"
#include "../Configuration/Configuration.h"
#include "../RGBDiode/RGBDiode.h"
#include "../Constants/Constants.h"
#define temperatureDownTolerationProgramme 2
#define warmingUpTime 40000

typedef struct {
  bool (*hasValidForecast)();
  void (*act)();
  bool (*shouldICool)();
} WeatherForecast;

typedef struct {
  ConfigurationData* data;
  float tempInside;
  float tempOutside;
  float humidityOutside;
  float dewPoint;
  WeatherForecast forecast;
  bool isTimeSet;
} ProgrammeContext;

typedef struct {
  byte mode = POWER_OUTPUT_MODE_NONE;
  byte ventilatorPower = 0;
  byte recuperationPower = 0;
  byte recuperationMode = RECUPERATION_MODE_RECYCLE;
} PowerOutput;

class Programme {
  public:
    int getDescription(char dest[80]);
    void onStart();
    boolean handleClick(short times);
    void handleHold(int duration_ms, boolean finished);
    void getPower(ProgrammeContext * context, PowerOutput *out);
    void configureTicking(RGBDiode *diode);
    boolean isValid(ConfigurationData* data);
    boolean canForce();
    byte getCode();
};

class ConfigurableProgramme: public Programme {
  public:
    void setPower(PowerOutput output, int duration);
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