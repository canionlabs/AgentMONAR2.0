#ifdef SENSOR_DHT
/*
* @Author: Ramon Melo
* @Date:   2018-08-06
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2018-08-13
*/

#include "SensorDHT.h"

// #define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

namespace monar
{
SensorDHT::SensorDHT(int pin) : dht(pin, DHTTYPE)
{
	dht.begin();

	sensor_t sensor;
	dht.temperature().getSensor(&sensor);
	Serial.println(F("------------------------------------"));
	Serial.println(F("Temperature Sensor"));
	Serial.print(F("Sensor Type: "));
	Serial.println(sensor.name);
	Serial.print(F("Driver Ver:  "));
	Serial.println(sensor.version);
	Serial.print(F("Unique ID:   "));
	Serial.println(sensor.sensor_id);
	Serial.print(F("Max Value:   "));
	Serial.print(sensor.max_value);
	Serial.println(F("°C"));
	Serial.print(F("Min Value:   "));
	Serial.print(sensor.min_value);
	Serial.println(F("°C"));
	Serial.print(F("Resolution:  "));
	Serial.print(sensor.resolution);
	Serial.println(F("°C"));
	Serial.println(F("------------------------------------"));
	// Print humidity sensor details.
	dht.humidity().getSensor(&sensor);
	Serial.println(F("Humidity Sensor"));
	Serial.print(F("Sensor Type: "));
	Serial.println(sensor.name);
	Serial.print(F("Driver Ver:  "));
	Serial.println(sensor.version);
	Serial.print(F("Unique ID:   "));
	Serial.println(sensor.sensor_id);
	Serial.print(F("Max Value:   "));
	Serial.print(sensor.max_value);
	Serial.println(F("%"));
	Serial.print(F("Min Value:   "));
	Serial.print(sensor.min_value);
	Serial.println(F("%"));
	Serial.print(F("Resolution:  "));
	Serial.print(sensor.resolution);
	Serial.println(F("%"));
	Serial.println(F("------------------------------------"));
}

void SensorDHT::service()
{
	// float v = ((float) ESP.getVcc() / 1024.0f) * 1.12;
	// setData(0, v);

	sensors_event_t event;
	dht.temperature().getEvent(&event);
	if (isnan(event.temperature))
	{
		Serial.println(F("Error reading temperature!"));
	}
	else
	{
		Serial.print(F("Temperature: "));
		Serial.print(event.temperature);
		Serial.println(F("°C"));

		setData(0, event.temperature);
	}
	// Get humidity event and print its value.
	dht.humidity().getEvent(&event);
	if (isnan(event.relative_humidity))
	{
		Serial.println(F("Error reading humidity!"));
	}
	else
	{
		Serial.print(F("Humidity: "));
		Serial.print(event.relative_humidity);
		Serial.println(F("%"));

		setData(1, event.relative_humidity);
	}
}

int SensorDHT::length()
{
	return 2;
}

char SensorDHT::prefix()
{
	return 'h';
}
} // namespace monar
#endif