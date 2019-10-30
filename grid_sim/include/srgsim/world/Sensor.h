#pragma once

#include "srgsim/containers/CellPerceptions.h"

#include <vector>

namespace srgsim {
    class World;
    class ServiceRobot;
    class Sensor {
    public:
        Sensor(ServiceRobot* robot);
        virtual ~Sensor();
        virtual std::vector<CellPerceptions> createPerceptions(World* world) = 0;
    protected:
        ServiceRobot* robot;
    };
}
