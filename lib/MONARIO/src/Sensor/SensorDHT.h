#ifdef SENSOR_DHT
#pragma once

#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

namespace monar {

  class SensorDHT : public Sensor
  {
  public:
    SensorDHT(int pin);

    void service() override;
    int length() override;
    char prefix() override;

    // void receive(int pin, int value) override;
    // void notify(void(*alert)(int, String, bool)) override;

  private:
	  DHT_Unified dht;
  };
}
#endif