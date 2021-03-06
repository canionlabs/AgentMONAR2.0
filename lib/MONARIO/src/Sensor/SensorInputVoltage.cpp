/*
* @Author: Ramon Melo
* @Date:   2018-08-06
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2018-08-13
*/

#include "SensorInputVoltage.h"

namespace monar {

  SensorInputVoltage::SensorInputVoltage() {}

  void SensorInputVoltage::service() {
    float v = ((float) ESP.getVcc() / 1024.0f) * 1.12;
    setData(0, v);
  }

  int SensorInputVoltage::length()
  {
    return 1;
  }

  char SensorInputVoltage::prefix()
  {
    return 'v';
  }
}
