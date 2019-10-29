#pragma once

#include "Actuator.h"

namespace monar
{
class ActuatorIR : public Actuator
{
public:
	ActuatorIR();
	void process(String data) override;
};
} // namespace monar