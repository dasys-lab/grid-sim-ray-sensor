#pragma once

namespace srgsim {
    class World;
    class ServiceRobot;
    class Actuator {
    public:
        Actuator(ServiceRobot* robot);
        virtual ~Actuator();
    protected:
        ServiceRobot* robot;
    };
}
