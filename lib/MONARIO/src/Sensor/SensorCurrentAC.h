/*
* @Author: Ramon Melo
* @Date:   2019-10-29
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2019-10-29
*/

#pragma once

#include "Sensor.h"

namespace monar {

  class SensorCurrentAC : public Sensor
  {
  public:
    SensorCurrentAC();

    void service() override;
    int length() override;
    char prefix() override;

//   private:
  };
}
