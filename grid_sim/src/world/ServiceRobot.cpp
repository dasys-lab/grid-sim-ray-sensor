#include "srgsim/world/ServiceRobot.h"

#include "srgsim/world/ObjectDetection.h"
#include "srgsim/world/Manipulation.h"

#include <essentials/IdentifierConstPtr.h>

namespace srgsim{

    ServiceRobot::ServiceRobot(essentials::IdentifierConstPtr id) : Object(Type::Robot, id) {
        this->objectDetection = new ObjectDetection(this);
        this->manipulation = new Manipulation(this);
    }

    SimPerceptions ServiceRobot::createSimPerceptions(World* world) {
        SimPerceptions sps;
        sps.receiverID = this->getID();

        // objects
        std::vector<CellPerceptions> objectPerceptions = this->objectDetection->createPerceptions(world);
        sps.cellPerceptions.insert(sps.cellPerceptions.end(), objectPerceptions.begin(), objectPerceptions.end());

        return sps;
    }

    void ServiceRobot::executeAction(SimCommand sc, World* world) {
        this->manipulation->manipulate(sc, world);
    }

    Object *ServiceRobot::getCarriedObject() {
        return this->manipulation->getCarriedObject();
    }
}
