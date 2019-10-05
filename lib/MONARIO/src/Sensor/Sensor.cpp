/*
* @Author: Ramon Melo
* @Date:   2018-07-24
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2018-08-20
*/

#include "Sensor.h"

namespace monar {
  Sensor::Sensor() {}

//   void Sensor::send(void (*push)(int, float)) {
//     this->service();

//     for (auto it = info.begin(); it != info.end(); ++it) {
//       (*push)(it->first, it->second);
//     }
//   }

  void Sensor::setData(int pin, float data) {
    info[pin] = data;
  }

  float Sensor::read(int idx)
  {
    return info[idx];
  }

//   void Sensor::receive(int pin, int value) {}
//   void Sensor::notify(void(*alert)(int, String, bool)) {}
}
