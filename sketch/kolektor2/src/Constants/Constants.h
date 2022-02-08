
#ifndef Constants_h
#define Constants_h
#define DEFAULT_SSID "SolarAirCollectorV2"
#define DEFAULT_PASSWORD "123456789"

#define IS_DEBUG true
#define TESTING_CO2_SENSOR true

#define temporaryDisabledProgrammeDuration 3600000 // 1hour
#define manual100ProgrammeDuration 900000 // 15 minutes
#define updatingSensorStatsInterval 60000

#if defined(IS_DEBUG)
#define averageValuesCount 1
#elif
#define averageValuesCount 7
#endif

#define INVALID_VALUE -9999
#define enablementTresholdMs 4000 // 4 seconds

#define syncForecastTolerateLastSuccessFor 172800000 // 2 days
#define syncForecastInterval 43200000 // 12 hours

#define POWER_OUTPUT_MODE_NONE 0
#define POWER_OUTPUT_MODE_VENTILATION 1
#define POWER_OUTPUT_MODE_RECUPERATION 2
#define POWER_OUTPUT_MODE_BOTH 3

#define RECUPERATION_MODE_RECYCLE 1
#define RECUPERATION_MODE_INHALE 2
#define RECUPERATION_MODE_EXHALE 3


#define RECUPERATION_WAIT_FOR_DIRECTION_CHANGE 4000
#if defined(IS_DEBUG)
#define RECUPERATION_CYCLE_DURATION 20000
#elif 
#define RECUPERATION_CYCLE_DURATION 70000
#endif
#define RELAY_COOLDOWN 5000
#endif

#define VENTILATOR_MAX_RPM 7600
#define RECUPERATION_MAX_RPM 5500

#define FAN_TYPE_VENTILATOR 1
#define FAN_TYPE_RECUPERATION 2
