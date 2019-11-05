#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "config.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <Sensor/SensorDallas.h>
#include <Sensor/SensorWallVoltage.h>

#define ONE_WIRE_BUS D5 // 14
#define VOLTAGE_SENSOR D0
#define LED_IN D4 // 2
#define LED D6	// 12

#define SECOND 1000
#define MINUTE SECOND * 60

// #define UPDATE_RATE MINUTE
// #define UPDATE_SENSOR_RATE SECOND * 5

#define CONNECT_WAIT 30 * SECOND // 30 sec
#define RECONNECT_RATE 5000
#define READ_RATE 15 * SECOND

enum CurrState
{
	INIT,
	WIFI_DISCONNECTED,
	WIFI_CONNECTED,
	BROKER_DISCONNECTED,
	BROKER_CONNECTED,
	SMART_CONFIG,
};

CurrState state = CurrState::INIT;

OneWire oneWire(ONE_WIRE_BUS);
WiFiClient espClient;
PubSubClient client(espClient);

std::vector<monar::Sensor *> sensors;

unsigned long last_up = 0;
bool status = false;
bool long_blink = false;

unsigned long nextReconnectAttempt = 0;
unsigned long nextSend = 0;

void init_blinker()
{
	pinMode(LED, OUTPUT);
	last_up = millis();
}

void blinker()
{
	if (state != CurrState::BROKER_CONNECTED)
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

void wifiConnect()
{
	if (WiFi.status() == WL_CONNECTED)
	{
		return;
	}

#if defined(WIFI_SSID) && defined(WIFI_PASS)
	WiFi.begin(WIFI_SSID, WIFI_PASS);
#endif

	WiFi.mode(WIFI_STA);

	Serial.println("Connecting to WIFI");
	state = CurrState::WIFI_DISCONNECTED;

	unsigned long wait_time = millis() + CONNECT_WAIT;

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(50);
		blinker();

		// wait 30 sec to connect to wifi
		if (wait_time < millis())
		{
			// enter SmartConfig mode
			WiFi.beginSmartConfig();
			state = CurrState::SMART_CONFIG;
			blinker();

			Serial.println("Begin smart config");

			wait_time = millis() + CONNECT_WAIT;

			while (true)
			{
				delay(50);

				if (WiFi.smartConfigDone())
				{
					break;
				}

				// wait 30 sec on smart config
				if (wait_time < millis())
				{
					// restart if smart config fails
					ESP.restart();
				}
			}
		}
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		Serial.printf("Connected, mac address: %s\n", WiFi.macAddress().c_str());
	}

	state = CurrState::WIFI_CONNECTED;
}

void brokerConnect()
{
	if (client.connected() || nextReconnectAttempt > millis())
	{
		return;
	}

	Serial.println("Connecting to Broker");

	state = CurrState::BROKER_DISCONNECTED;

	if (client.connect(MQTT_ID, MQTT_USER, NULL))
	{
		state = CurrState::BROKER_CONNECTED;

		// Once connected, publish an announcement...
		// client.publish(MES_STATE_TOPIC, MES_DEVICE_ID);
		// client.subscribe(MES_CFG_TOPIC);

		Serial.println("Connected to Broker");
	} else {
		Serial.print("Error while connecting to broker: ");
		Serial.println(client.state());
	}

	nextReconnectAttempt = millis() + RECONNECT_RATE;
}

void buildMessage(String *jsonStr)
{
	const size_t bufferSize = JSON_OBJECT_SIZE(4);
	DynamicJsonDocument jsonBuffer(bufferSize);

	for (unsigned int i = 0; i < sensors.size(); ++i)
	{
		for (int j = 0; j < sensors.at(i)->length(); ++j)
		{
			String base = "";
			base += sensors.at(i)->prefix();
			base += j;

			Serial.println(base);
			Serial.println(sensors.at(i)->read(j));

			jsonBuffer[base] = sensors.at(i)->read(j);
		}
	}

	serializeJson(jsonBuffer, *jsonStr);
}

void serviceSensor()
{
    for (unsigned int i = 0; i < sensors.size(); ++i)
    {
        sensors.at(i)->service();
    }
}

void sendEvent()
{
	// Cancel if not connected
	if (client.connected() == false) { return; }

	// Wait for update rate
	if (nextSend < millis())
	{
		serviceSensor();

		String msg;
		buildMessage(&msg);

		client.publish(DATA_TOPIC, msg.c_str());

		nextSend = millis() + READ_RATE;

		Serial.println("push");
	}
}

void setup()
{
	Serial.begin(115200);
	while (!Serial) {}

	state = CurrState::INIT;
	init_blinker();

	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);

	client.setServer(MQTT_BROKER, MQTT_PORT);

	sensors.push_back(new monar::SensorDallas(&oneWire));
	sensors.push_back(new monar::SensorWallVoltage(VOLTAGE_SENSOR));
	
	Serial.println("ready");
}

void loop()
{
	blinker();

	wifiConnect();
	brokerConnect();
	client.loop();

	sendEvent();

	delay(10);
}