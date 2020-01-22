#pragma once

#include "srgsim/containers/CellPerceptions.h"
#include "Sensor.h"

#include <vector>
#include <SystemConfig.h>

namespace essentials{
    class SystemConfig;
}

namespace srgsim {
    class Cell;
    class Coordinate;
    class ObjectDetection : public Sensor {
    public:
        ObjectDetection(ServiceRobot* robot);
        std::vector<CellPerceptions> createPerceptions(World* world);

    private:
        std::vector<const Cell*> collectCells(Coordinate p0, Coordinate p1, World* world);
        std::vector<const Coordinate> collectCoordinates(Coordinate &start, Coordinate &end);

        essentials::SystemConfig* sc;
        uint32_t sightLimit;
    };
}