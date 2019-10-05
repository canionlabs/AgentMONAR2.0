#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "config.h"

#define ONE_WIRE_BUS D5 // 14
#define VOLTAGE_SENSOR D0
#define LED_IN D4 // 2
#define LED D6	// 12

#define SECOND 1000
#define MINUTE SECOND * 60

#define UPDATE_RATE MINUTE
#define UPDATE_SENSOR_RATE SECOND * 5

bool connected = false;
unsigned long last_up = 0;
bool status = false;
bool long_blink = false;

void init_blinker()
{
	pinMode(LED, OUTPUT);
	last_up = millis();
}

void blinker()
{
	if (!connected)
	{
		if (millis() > last_up)
		{
			status = !status;

			last_up = millis() + (long_blink ? 500 : 100);
		}

		digitalWrite(LED, status);
	}
	else
	{
		digitalWrite(LED, HIGH);
	}
}

void connect()
{
	WiFi.mode(WIFI_STA);

#ifdef WIFI_NAME
#ifdef WIFI_PASS
	WiFi.begin(WIFI_NAME, WIFI_PASS);
	Serial.println("Connecting with credentials...");
#endif
#endif

	bool success = false;

	Serial.println("Connecting...");

	long_blink = true;

	long start_time = millis();
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(10);

		if (((millis() - start_time) > MINUTE) && !success)
		{
			long_blink = false;

			WiFi.beginSmartConfig();
			Serial.println("Begin SmartConfig...");

			while (1)
			{
				delay(10);

				if (WiFi.smartConfigDone())
				{
					Serial.println("SmartConfig: Success");

					success = true;
					break;
				}

				if ((millis() - start_time) > (MINUTE * 2))
				{
					ESP.restart();
				}

				blinker();
			}
		}

		blinker();
	}

	long_blink = true;

	WiFi.printDiag(Serial);
	// Serial.print("IP Address: "); Serial.println(WiFi.localIP().toString().c_str());
	// Serial.print("Gateway IP: "); Serial.println(WiFi.gatewayIP().toString().c_str());
	// Serial.print("Hostname: "); Serial.println(WiFi.hostname().c_str());
	// Serial.print("RSSI: "); Serial.println(WiFi.RSSI());
}

void setup()
{
	delay(500);
	Serial.begin(9600);

	init_blinker();

	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
	connect();

	// initialize LED digital pin as an output.
	pinMode(LED, OUTPUT);
}

void loop()
{
	blinker();
}