
#pragma once

#include "Sensor.h"

namespace monar {

  class SensorWallVoltage : public Sensor
  {
  public:
    SensorWallVoltage(int pin);

    void service() override;
    int length() override;
    char prefix() override;
    // void notify(void(*alert)(int, String, bool)) override;

  private:
    int inputPin;
  };
}
