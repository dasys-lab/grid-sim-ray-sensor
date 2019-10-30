#include "srgsim/world/Sensor.h"

#include "srgsim/world/ServiceRobot.h"

namespace srgsim
{
Sensor::Sensor(ServiceRobot* robot)
        : robot(robot)
{
}

Sensor::~Sensor() {}
} // namespace srgsim
