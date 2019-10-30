#pragma once

#include "srgsim/containers/Coordinate.h"

#include "srgsim/SRGEnums.h"
#include "srgsim/containers/SimPerceptions.h"

#include <essentials/IdentifierConstPtr.h>

#include <map>
#include <mutex>
#include <srgsim/containers/SimCommand.h>
#include <stdint.h>
#include <unordered_map>
#include <vector>

namespace essentials
{
class SystemConfig;
}
namespace srgsim
{
class Cell;
class Object;
class ServiceRobot;

/**
 * Coordinates are:
 * X (left to right)
 * Y (bottom to up)
 */
class World
{
public:
    World();
    World(std::string tmxMapFile);
    ~World();

    Cell* addCell(uint32_t x, uint32_t y);
    const Cell* getCell(Coordinate coordinate) const;

    uint32_t getSizeX() const;
    uint32_t getSizeY() const;
    const std::map<Coordinate, Cell*>& getGrid();
    std::vector<Perception>& getMarkers();
    void addMarker(Perception p);
    std::recursive_mutex& getDataMutex();

    std::vector<SimPerceptions> createSimPerceptionsList();

    std::vector<Object*> updateCell(srgsim::CellPerceptions cellPerceptions);

    // objects
    const Object* getObject(essentials::IdentifierConstPtr id);
    Object* editObject(essentials::IdentifierConstPtr id);
    Object* createOrUpdateObject(essentials::IdentifierConstPtr id, Type type, State state = State::Undefined);
    bool removeObject(Object* object);
    bool placeObject(Object* object, Coordinate coordinate);
    void moveObject(essentials::IdentifierConstPtr id, Direction direction);

    // robots
    bool spawnRobot(essentials::IdentifierConstPtr id);
    ServiceRobot* getRobot(essentials::IdentifierConstPtr id);
    bool addRobot(ServiceRobot* robot);

    // other
    void openDoor(essentials::IdentifierConstPtr id);
    void closeDoor(essentials::IdentifierConstPtr id);

private:
    bool isPlacementAllowed(const Cell* cell, Type objectType) const;
    Cell* getNeighbourCell(const Direction& direction, Object* object);


    std::map<Coordinate, Cell*> cellGrid;
    /**
     * Current field length
     */
    uint32_t sizeX;
    /**
     * Current field width
     */
    uint32_t sizeY;

    mutable std::recursive_mutex dataMutex;
    std::unordered_map<essentials::IdentifierConstPtr, Object*> objects;
    std::unordered_map<essentials::IdentifierConstPtr, ServiceRobot*> robots;
    std::vector<Perception> markers;
};
} // namespace srgsim