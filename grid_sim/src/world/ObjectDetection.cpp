#include "srgsim/world/ObjectDetection.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/containers/Coordinate.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"
#include "srgsim/containers/CellPerceptions.h"

#include <SystemConfig.h>
#include <cnc_geometry/Calculator.h>

#include <math.h>

namespace srgsim
{
ObjectDetection::ObjectDetection(srgsim::ServiceRobot* robot)
        : Sensor(robot)
{
    this->sc = essentials::SystemConfig::getInstance();
    this->sightLimit = (*sc)["ObjectDetection"]->get<uint32_t>("sightLimit", NULL);
}

std::vector<CellPerceptions> ObjectDetection::createPerceptions(World* world)
{
    std::map<Coordinate, Cell*> grid = world->getGrid();
    std::vector<CellPerceptions> cellPerceptionsList;

    std::map<Coordinate, Cell*>::iterator it;
    for ( it = grid.begin(); it != grid.end(); it++ )
    {
        CellPerceptions cellPerceptions;

        std::cout << it->first  // string (key)
                  << ':'
                  << it->second   // string's value
                  << std::endl ;

        cellPerceptions.x = it->first.x;
        cellPerceptions.y = it->first.y;
        std::vector<Object*> objects = it->second->getObjects();

        for(int i = 0; i < objects.size(); i++){
            Perception* perception = new Perception();
            Object* object = objects.at(i);
            perception->state = object->getState();
            perception->type = object->getType();
            perception->objectID = object->getID();
            perception->x = cellPerceptions.x;
            perception->y = cellPerceptions.y;
            cellPerceptions.perceptions.push_back(*perception);
        }

        cellPerceptionsList.push_back(cellPerceptions);
    }

    return cellPerceptionsList;
}

std::vector<const Cell*> ObjectDetection::collectCells(Coordinate start, Coordinate end, World* world)
{
    std::vector<const Cell*> cells;

    std::map<Coordinate, Cell *> grid = world->getGrid();
    world->getCell(start);

    //TODO implement me
    return cells;
}
} // namespace srgsim
