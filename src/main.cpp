#include "config.h"

#include "Arduino.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>

// AutoConnect
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>

#include <Sensor/Sensor.h>

#ifdef SENSOR_DALLAS
#include <Sensor/SensorDallas.h>

#define ONE_WIRE_BUS D5 // 14
OneWire oneWire(ONE_WIRE_BUS);
#endif

#ifdef SENSOR_WALL
#include <Sensor/SensorWallVoltage.h>
#define VOLTAGE_SENSOR D0
#endif

#ifdef SENSOR_DHT
#include <Sensor/SensorDHT.h>
#define DHT_PIN D3
#endif

#define LED_IN D4  // 2
#define LED LED_IN // D6	// 12

#define SECOND 1000
#define MINUTE SECOND * 60

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

WiFiClient espClient;
PubSubClient client(espClient);

ESP8266WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig Config;

std::vector<monar::Sensor *> sensors;

unsigned long last_up = 0;
bool status = false;
bool long_blink = false;

bool sent_attr = false;

unsigned long nextReconnectAttempt = 0;
unsigned long nextSend = 0;

void rootPage()
{
	char content[] = "MONAR, CanionLabs";
	Server.send(200, "text/plain", content);
}

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
		Serial.println("Connected to Broker");
	}
	else
	{
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
	if (client.connected() == false)
	{
		return;
	}

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

void update_attr()
{
	if (sent_attr == false && client.connected())
	{
		String output;
		StaticJsonDocument<200> doc;
		doc["v"] = VERSION;
		serializeJson(doc, output);

		Serial.println(output);

		if (client.publish(ATTR_TOPIC, output.c_str()))
		{
			Serial.println("Attributes Updated");
			sent_attr = true;
		}
	}
}

void setup()
{
	Serial.begin(115200);
	while (!Serial)
	{
	}

	Serial.println("\nstarting...");

	state = CurrState::INIT;
	init_blinker();

	client.setServer(MQTT_BROKER, MQTT_PORT);

#ifdef SENSOR_DALLAS
	sensors.push_back(new monar::SensorDallas(&oneWire));
#endif
#ifdef SENSOR_WALL
	sensors.push_back(new monar::SensorWallVoltage(VOLTAGE_SENSOR));
#endif
#ifdef SENSOR_DHT
	sensors.push_back(new monar::SensorDHT(DHT_PIN));
#endif

	Config.title = PORTAL_TITLE;
	Config.apid = PORTAL_TITLE + WiFi.macAddress();
	Config.psk = PORTAL_PW;
	Config.autoReconnect = true;
	Config.autoReset = true;

	Portal.config(Config);

	Server.on("/", rootPage);
	if (Portal.begin())
	{
		Serial.println("WiFi connected: " + WiFi.localIP().toString());
	}
	else
	{
		Serial.println("Error starting portal");
	}

	Serial.println("ready");
}

void loop()
{
	blinker();

	Portal.handleClient();

	brokerConnect();
	client.loop();

	sendEvent();
	update_attr();

	delay(10);
}