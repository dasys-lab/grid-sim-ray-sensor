#pragma once

#include "Actuator.h"
#include "srgsim/containers/SimCommand.h"

#include <essentials/IdentifierConstPtr.h>

#include <vector>

namespace srgsim {
    class Object;
    class Manipulation : public Actuator{
    public:
        Manipulation(ServiceRobot* robot);
        bool manipulate(SimCommand sc, srgsim::World* world);

        Object *getCarriedObject();

    private:
        bool open(essentials::IdentifierConstPtr objectID, srgsim::World* world);
        bool close(essentials::IdentifierConstPtr objectID, srgsim::World* world);
        bool pickUp(essentials::IdentifierConstPtr objectID, srgsim::World* world);
        bool putDown(SimCommand sc, srgsim::World* world);

        Object* carriedObject;
    };
}