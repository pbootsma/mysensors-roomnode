/*
  Config.h - Config file for Roomnode sketch
*/

#ifndef RoomnodeConfig_h
#define RoomnodeConfig_h

// Default settings
#define ROOMNODE_VERSION "1.4"
#define ROOMNODE_SEND_INTERVAL 300000 // Default 5 minutes
//#define ROOMNODE_POWER_DOWN_I2C
//#define ROOMNODE_SENSORS_POWER_PIN A2
//#define ROOMNODE_IS_REPEATER

//#define ROOMNODE_HAS_SI7021
//#define ROOMNODE_HAS_MQ_135

#define ROOMNODE_MQ_135_MAX_PPM 10000
#define ROOMNODE_MQ_135_PIN A0
#define ROOMNODE_MQ_135_FACTOR (146.0/(146+100))

//#define ROOMNODE_BATTERY_POWERED
#define ROOMNODE_BATTERY_SAMPLE_COUNT 5
#define ROOMNODE_BATTERY_FULL_VOLT 2.8
#define ROOMNODE_BATTERY_EMPTY_VOLT 1.8

// Define which roomnode config to use
//#define ROOMNODE_WOONKAMER
//#define ROOMNODE_BATHROOM
//#define ROOMNODE_REFRIGERATOR
//#define ROOMNODE_FREEZER
//#define ROOMNODE_MASTER_BEDROOM
#define ROOMNODE_AIR_QUALITY

// Roomnode specific settings

// Test
#ifdef ROOMNODE_WOONKAMER
	#define ROOMNODE_NAME "Roomnode"
	#define VOLTAGE_DIVIDER_HIGH 1000
	#define VOLTAGE_DIVIDER_LOW 333
	#define ROOMNODE_SENSORS_POWER_PIN A2
	#define ROOMNODE_POWER_DOWN_I2C
	#define ROOMNODE_HAS_SI7021
	#define ROOMNODE_BATTERY_POWERED
#endif

// Master Bedroom
#ifdef ROOMNODE_AIR_QUALITY
	#define ROOMNODE_NAME "Air quality"
	#define ROOMNODE_SEND_INTERVAL 60000
	#define ROOMNODE_HAS_MQ_135
	#define ROOMNODE_HAS_SI7021
	#define ROOMNODE_IS_REPEATER
#endif

// Refrigerator
#ifdef ROOMNODE_REFRIGERATOR
	#define ROOMNODE_NAME "Koelkast"
	#define VOLTAGE_DIVIDER_HIGH 987
	#define VOLTAGE_DIVIDER_LOW 335
	#define ROOMNODE_BATTERY_FULL_VOLT 4.0
	#define ROOMNODE_BATTERY_EMPTY_VOLT 3.0
	#define ROOMNODE_SENSORS_POWER_PIN 7
	#define ROOMNODE_POWER_DOWN_I2C
	#define ROOMNODE_BATTERY_POWERED
#endif

// Freezer
#ifdef ROOMNODE_FREEZER
	#define ROOMNODE_NAME "Diepvries"
	#define ROOMNODE_BATTERY_POWERED
	#define ROOMNODE_SENSORS_POWER_PIN A2
	#define VOLTAGE_DIVIDER_HIGH 1000
	#define VOLTAGE_DIVIDER_LOW 470
	#define ROOMNODE_HAS_SI7021
#endif

// Bathroom
#ifdef ROOMNODE_BATHROOM
	#define ROOMNODE_NAME "Badkamer"
	#define ROOMNODE_BATTERY_POWERED
	#define ROOMNODE_SENSORS_POWER_PIN A2
	#define VOLTAGE_DIVIDER_HIGH 1000
	#define VOLTAGE_DIVIDER_LOW 470
	#define ROOMNODE_SEND_INTERVAL 60000
	#define ROOMNODE_HAS_SI7021
#endif

// Master Bedroom
#ifdef ROOMNODE_MASTER_BEDROOM
	#define ROOMNODE_NAME "Slaapkamer"
	#define ROOMNODE_HAS_SI7021
	#define ROOMNODE_SEND_INTERVAL 60000
	#define ROOMNODE_IS_REPEATER
#endif

#define VOLTAGE_DIVIDER_FACTOR ((VOLTAGE_DIVIDER_HIGH + VOLTAGE_DIVIDER_LOW)/(VOLTAGE_DIVIDER_LOW * 1.0))

#endif // RoomnodeConfig_h