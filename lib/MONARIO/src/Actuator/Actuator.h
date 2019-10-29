#pragma once

#include "Arduino.h"

namespace monar {

  class Actuator {
  public:
    Actuator();
    virtual void process(String data) = 0;
  };
}
