/*
  Roomnode.ino - Sketch for MySensors temperature and humidity node
*/

#include "Config.h"

#include <SPI.h>
#include <Wire.h>
#include <MySensors.h>
#include <avr/power.h>
#include "Si7021.h"

#define CHILD_ID_TEMP 0
#define CHILD_ID_HUM 1
#define CHILD_ID_VOLT 2
#define CHILD_ID_MQ_135 3

Si7021 tempHumiditySensor;

#ifdef ROOMNODE_HAS_SI7021
	MyMessage msgHum(CHILD_ID_HUM, V_HUM);
	MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
#endif
#ifdef ROOMNODE_BATTERY_POWERED
	MyMessage msgVolt(CHILD_ID_VOLT, V_VOLTAGE);
#endif
#ifdef ROOMNODE_HAS_MQ_135
	MyMessage msgMq135(CHILD_ID_MQ_135, V_LEVEL);
#endif

int RAW_VOLT_PIN = A0;

unsigned long lastSendTime = 0;

void setup()
{
	Serial.println("Sketch setup");

	// Blink
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);

	// Use the 1.1 V internal reference
	analogReference(INTERNAL);

	#ifdef ROOMNODE_SENSORS_POWER_PIN
		// Power up Si7021
		pinMode(ROOMNODE_SENSORS_POWER_PIN, OUTPUT);
		digitalWrite(ROOMNODE_SENSORS_POWER_PIN, HIGH);
	#endif
	
	Serial.println("Sketch setup done");
}

void presentation() 
{
  // Send the Sketch Version Information to the Gateway
  sendSketchInfo(ROOMNODE_NAME, ROOMNODE_VERSION);

  // Register all sensors to gw (they will be created as child devices)
  #ifdef ROOMNODE_HAS_SI7021
    present(CHILD_ID_HUM, S_HUM, "Si7021");
    present(CHILD_ID_TEMP, S_TEMP, "Si7021");
    
    // Start  Si7021
    tempHumiditySensor.begin();
  #endif
  
  #ifdef ROOMNODE_HAS_MQ_135
    present(CHILD_ID_MQ_135, S_AIR_QUALITY, "MQ-135");
  #endif
  
  #ifdef ROOMNODE_BATTERY_POWERED
    present(CHILD_ID_VOLT, S_MULTIMETER, "Battery");
  #endif
}

void loop()
{
	unsigned long loopStart = millis();
	
	#ifdef MY_REPEATER_FEATURE
		if ((loopStart - lastSendTime) > ROOMNODE_SEND_INTERVAL) {
			readSensorsAndSendData();
			lastSendTime = loopStart;
		}
	#else
		readSensorsAndSendData();
		sleep(ROOMNODE_SEND_INTERVAL);
	#endif
}

void readSensorsAndSendData() {
	
	// Power up i2c
	#ifdef ROOMNODE_POWER_DOWN_I2C	
		powerUpI2C();
	#endif
	
	#ifdef ROOMNODE_HAS_SI7021
		// Read temp and humidity
		bool tempHumidityRead = tempHumiditySensor.read();
	
		if (tempHumidityRead) {
			// Send temperature through RF
			send(msgTemp.set(tempHumiditySensor.temperature, 1));
		
			// Send Humidity through RF
			send(msgHum.set(tempHumiditySensor.humidity, 1));
		}
	#endif

	// Power down i2c
	#ifdef ROOMNODE_POWER_DOWN_I2C
		powerDownI2C();
	#endif
	
	#ifdef ROOMNODE_HAS_MQ_135
		int mq135Value = analogRead(ROOMNODE_MQ_135_PIN);
		int mq135Ppm = (mq135Value/1023.0) * ROOMNODE_MQ_135_MAX_PPM;
		send(msgMq135.set(mq135Ppm));
	#endif

	#ifdef ROOMNODE_BATTERY_POWERED
		// Battery voltage
		float voltValue = 0;
		for (int i=0; i<ROOMNODE_BATTERY_SAMPLE_COUNT; i++) {
			voltValue += analogRead(RAW_VOLT_PIN)/(ROOMNODE_BATTERY_SAMPLE_COUNT * 1.0);
		}
		float volt = voltValue/1023.0 * 1.1 * VOLTAGE_DIVIDER_FACTOR;
		send(msgVolt.set(volt, 3));
		
		// Battery percentage
		int batteryPcnt = 100*(volt - ROOMNODE_BATTERY_EMPTY_VOLT)/(ROOMNODE_BATTERY_FULL_VOLT-ROOMNODE_BATTERY_EMPTY_VOLT);
		if (batteryPcnt > 100) batteryPcnt = 100;
		sendBatteryLevel(batteryPcnt);
	#endif
}

void powerDownI2C()
{
	power_twi_disable();
	
	//Power down 7021
	#ifdef ROOMNODE_SENSORS_POWER_PIN
		digitalWrite(ROOMNODE_SENSORS_POWER_PIN, LOW);
	#endif
	
	// Disable digital input buffers on A0 to A5 pins
    //DIDR0 = 0x3F; 
    
    pinMode(SDA, INPUT);
    pinMode(SCL, INPUT);
    digitalWrite(SDA, LOW);
    digitalWrite(SCL, LOW);
}

void powerUpI2C()
{
	//Power up I2C SDA SCL
	digitalWrite(SDA, HIGH);
	digitalWrite(SCL, HIGH);

	//Power up Si7021 and wait for stable state
	#ifdef ROOMNODE_SENSORS_POWER_PIN
		digitalWrite(ROOMNODE_SENSORS_POWER_PIN, HIGH);
	#endif

	power_twi_enable();
	delay(100);
}
