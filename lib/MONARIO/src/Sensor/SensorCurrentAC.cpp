/*
* @Author: Ramon Melo
* @Date:   2019-10-29
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2019-10-29
*/

#include "SensorCurrentAC.h"

namespace monar {

  SensorCurrentAC::SensorCurrentAC() {}

  void SensorCurrentAC::service() {
    // float v = ((float) ESP.getVcc() / 1024.0f) * 1.12;
    // setData(0, v);
  }

  int SensorCurrentAC::length()
  {
    return 1;
  }

  char SensorCurrentAC::prefix()
  {
    return 'c';
  }
}
