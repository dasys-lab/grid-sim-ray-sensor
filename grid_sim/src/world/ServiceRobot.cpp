#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"
#include "srgsim/world/ObjectDetection.h"
#include "srgsim/world/Manipulation.h"

#include <essentials/IdentifierConstPtr.h>
#include <srgsim/world/Cell.h>

namespace srgsim {
    bool output = true;

    ServiceRobot::ServiceRobot(essentials::IdentifierConstPtr id) : Object(Type::Robot, id) {
        this->objectDetection = new ObjectDetection(this);
        this->manipulation = new Manipulation(this);
    }

    SimPerceptions ServiceRobot::createSimPerceptions(World *world) {
        SimPerceptions sps;
        sps.receiverID = this->getID();

        // objects
        std::vector<CellPerceptions> objectPerceptions = this->objectDetection->createPerceptions(world);
        sps.cellPerceptions.insert(sps.cellPerceptions.end(), objectPerceptions.begin(), objectPerceptions.end());

        return sps;
    }

    std::vector<const Cell *> ServiceRobot::castRay(World *world, float angleIncrement, float angleLimit) {
        // objects
        float currentAngle = 0;
        Coordinate rayStart = this->getCell()->coordinate;
        std::vector<const Cell *> cells;
        std::vector<const Cell *> result;

        while (currentAngle < angleLimit) {
            Coordinate rayEnd = this->objectDetection->getRayEndpoint(currentAngle, rayStart);
            currentAngle += angleIncrement;
            cells = this->objectDetection->collectCells(rayStart, rayEnd, world);

            for (int i = 0; i < cells.size(); i++) {
                const Cell *cell = cells.at(i);
                if (cell == NULL) {
                    // this means the cell is out of bounds and does not exist in the grid
                    break;
                }

                const std::map<Coordinate, Cell *> &grid = world->getGrid();

                srgsim::Type type = cell->type;

                if (!cell->getObjects().empty()) {
                    type = cell->getObjects().at(0)->getType();
                }

                if (type == srgsim::Type::Wall) {
                    result.push_back(cell);
                    if (output) {
                        std::cout << "Wall detected at: " << cell->coordinate << std::endl;
                    }
                    break;
                } else if (type == srgsim::Type::CupBlue) {
                    result.push_back(cell);
                    if (output) {
                        std::cout << "Blue cup detected at: " << cell->coordinate << std::endl;
                    }
                    break;
                } else if (type == srgsim::Type::CupRed) {
                    result.push_back(cell);
                    if (output) {
                        std::cout << "Red cup detected at: " << cell->coordinate << std::endl;
                    }
                    break;
                } else if (type == srgsim::Type::CupYellow) {
                    result.push_back(cell);
                    if (output) {
                        std::cout << "Yellow cup detected at: " << cell->coordinate << std::endl;
                    }
                    break;
                } else if (type == srgsim::Type::Door) {
                    result.push_back(cell);
                    if (output) {
                        std::cout << "Door detected at: " << cell->coordinate << std::endl;
                    }
                    break;
                } else if (type == srgsim::Type::Robot) {
                    result.push_back(cell);
                    if (output) {
                        std::cout << "Robot detected at: " << cell->coordinate << std::endl;
                    }
                    break;
                }
            }
        }

        output = false;
        return result;
    }

    ServiceRobot::~ServiceRobot() {

    }

    void ServiceRobot::executeAction(SimCommand sc, World *world) {
        this->manipulation->manipulate(sc, world);
    }

    Object *ServiceRobot::getCarriedObject() {
        return this->manipulation->getCarriedObject();
    }

}
