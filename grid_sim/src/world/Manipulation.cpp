#include "srgsim/world/Manipulation.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"

namespace srgsim
{
Manipulation::Manipulation(srgsim::ServiceRobot* robot)
        : Actuator(robot)
        , carriedObject(nullptr)
{
}

bool Manipulation::manipulate(SimCommand sc, World* world)
{
    switch (sc.action) {
    case SimCommand::OPEN:
        this->open(sc.objectID, world);
        break;
    case SimCommand::CLOSE:
        this->close(sc.objectID, world);
        break;
    case SimCommand::PICKUP:
        this->pickUp(sc.objectID, world);
        break;
    case SimCommand::PUTDOWN:
        this->putDown(sc, world);
        break;
    default:
        return false;
    }
    return true;
}

bool Manipulation::open(essentials::IdentifierConstPtr objectID, World* world)
{
    world->openDoor(objectID);
    return true;
}

bool Manipulation::close(essentials::IdentifierConstPtr objectID, World* world)
{
    world->closeDoor(objectID);
    return true;
}

bool Manipulation::pickUp(essentials::IdentifierConstPtr objectID, srgsim::World* world)
{
    if (this->carriedObject) {
        std::cout << "Manipulation::pickUp(): Already carrying!" << std::endl;
        return false;
    }

    auto object = world->editObject(objectID);
    if (!object){
        std::cout << "Manipulation::pickUp(): Object does not exist! ID: "  << *objectID << std::endl;
        return false;
    }
    object->deleteCell();
    this->carriedObject = object;
    return true;
}

bool Manipulation::putDown(SimCommand sc, srgsim::World* world)
{
    if (!this->carriedObject) {
        std::cout << "Manipulation::pickUp(): Nothing to put down!" << std::endl;
        return false;
    }

    if (world->placeObject(this->carriedObject, Coordinate(sc.x, sc.y))) {
        this->carriedObject = nullptr;
        return true;
    } else {
        std::cout << "Manipulation::pickUp(): Could not place the object here (" << sc.x << ", " << sc.y << ")" << std::endl;
        return false;
    }
}

Object* Manipulation::getCarriedObject()
{
    return this->carriedObject;
}
} // namespace srgsim