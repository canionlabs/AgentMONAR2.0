#pragma once

#include "Sensor.h"

namespace monar {

  class SensorInputVoltage : public Sensor
  {
  public:
    SensorInputVoltage();

    void service() override;
    int length() override;
    char prefix() override;
  };
}
