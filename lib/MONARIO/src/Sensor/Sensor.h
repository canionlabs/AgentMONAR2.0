#pragma once

#include <monar_defines.h>
#include <map>
#include "Arduino.h"

namespace monar {

  class Sensor {
  public:
    Sensor();

    void send(void (*push)(int, float));
    void setData(int pin, float data);

    virtual void receive(int pin, int value);
    virtual void service() = 0;
    virtual void notify(void(*alert)(int, String, bool));

  protected:
    std::map<int, float> info;
  };
}
