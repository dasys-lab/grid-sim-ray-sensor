#include "srgsim/world/Cell.h"

#include "srgsim/world/Object.h"

#include <iostream>
#include <sstream>

namespace srgsim
{
Cell::Cell(uint32_t x, uint32_t y)
        : coordinate(x, y)
{
    this->up = nullptr;
    this->down = nullptr;
    this->left = nullptr;
    this->right = nullptr;
    this->type = Type::Unknown;
}

Cell::Cell(const srgsim::Cell& cell)
        : coordinate(cell.coordinate)
{
    this->room = cell.room;
    this->type = cell.type;
    this->up = cell.up;
    this->down = cell.down;
    this->left = cell.left;
    this->right = cell.right;
    this->objects = cell.objects;
}

const std::vector<Object*>& Cell::getObjects() const
{
    return objects;
}

void Cell::addObject(Object* object)
{
    for (Object* o : objects) {
        if (o->getID() == object->getID()) {
            return;
        }
    }
    objects.push_back(object);
    object->setCell(this);
}
void Cell::removeObject(Object* object)
{
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects.at(i)->getID() == object->getID()) {
            objects.erase(objects.begin() + i);
            object->deleteCell();
            return;
        }
    }
}

void Cell::update(std::vector<Object*> updateObjects) {
    // remove unseen objects
    for (Object* cellObject : this->objects) {
        bool found = false;
        for (Object* updateObject : updateObjects) {
            if (updateObject->getID() == cellObject->getID()) {
                found = true;
                break;
            }
        }
        if (!found) {
            this->removeObject(cellObject);
        }
    }

    // add new objects
    for (Object* updateObject : updateObjects) {
        bool found = false;
        for (Object* cellObject : this->objects) {
            if (updateObject->getID() == cellObject->getID()) {
                found = true;
                break;
            }
        }
        if (!found) {
            this->addObject(updateObject);
        }
    }
}

std::string Cell::toString() const
{
    std::stringstream ss;
    ss << "Cell: (" << this->coordinate.x << ", " << this->coordinate.y << ") Type: " << static_cast<int>(this->type) << std::endl;
    return ss.str();
}

} // namespace srgsim
