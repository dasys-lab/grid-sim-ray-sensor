#include "srgsim/world/Actuator.h"

#include "srgsim/world/ServiceRobot.h"

namespace srgsim
{
    Actuator::Actuator(ServiceRobot* robot)
            : robot(robot)
    {
    }

    Actuator::~Actuator() {}
} // namespace srgsim
