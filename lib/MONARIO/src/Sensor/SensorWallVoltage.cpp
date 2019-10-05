/*
* @Author: Ramon Melo
* @Date:   2018-08-20
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2018-08-20
*/

#include "SensorWallVoltage.h"

namespace monar {

  SensorWallVoltage::SensorWallVoltage(int pin) : inputPin(pin) {
    pinMode(inputPin, INPUT);
  }

  void SensorWallVoltage::service() {
    int value = digitalRead(inputPin);
    setData(MONAR_OUTPUT_CURRENT, value);
  }

  void SensorWallVoltage::notify(void(*alert)(int, String, bool)) {
    if ( (int) info[MONAR_OUTPUT_CURRENT] == 0 ) {
      (*alert)(MONAR_OUTPUT_LOG, String("Alerta: Ausência de energia detectada"), true);
    }
  }
}
