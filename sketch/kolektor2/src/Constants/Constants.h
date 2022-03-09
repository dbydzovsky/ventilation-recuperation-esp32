
#ifndef Constants_h
#define Constants_h
#define DEFAULT_SSID "SolarAirCollectorV2"
#define DEFAULT_PASSWORD "123456789"

#define IS_DEBUG true
#define TESTING_CO2_SENSOR true

#define RPM_SAMPLING_DURATION 1000
#define updatingSensorStatsInterval 60000


#define averageValuesCount 7

#define INVALID_VALUE -9999
#define enablementTresholdMs 4000 // 4 seconds

#define POWER_OUTPUT_MODE_NONE 0
#define POWER_OUTPUT_MODE_VENTILATION 1
#define POWER_OUTPUT_MODE_RECUPERATION 2
#define POWER_OUTPUT_MODE_BOTH 3

#define RECUPERATION_MODE_RECYCLE 1
#define RECUPERATION_MODE_INHALE 2
#define RECUPERATION_MODE_EXHALE 3


#define FAN_TYPE_VENTILATOR 1
#define FAN_TYPE_RECUPERATION 2
#endif