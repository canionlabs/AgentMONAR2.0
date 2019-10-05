#pragma once

#include <monar_defines.h>
#include <map>
#include "Arduino.h"

namespace monar {

  class Sensor {
  public:
    Sensor();

    virtual void service() = 0;
    virtual int length() = 0;
    virtual char prefix() = 0;

    // void send(void (*push)(int, float));
    float read(int idx);
    void setData(int pin, float data);
    // virtual void receive(int pin, int value);
    // virtual void notify(void(*alert)(int, String, bool));

  protected:
    std::map<int, float> info;
  };
}
