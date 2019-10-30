#pragma once

#include "containers/SimCommand.h"

#include "SRGEnums.h"

#include <essentials/IdentifierConstPtr.h>

#include <queue>
#include <mutex>

namespace std
{
class thread;
}

namespace essentials
{
class IDManager;
class SystemConfig;
}

namespace srgsim
{

namespace communication
{
class Communication;
}

namespace commands
{
class CommandHandler;
}

class Object;
class World;
class GUI;
class Cell;

class Simulator
{
public:
    Simulator(bool headless = false);
    ~Simulator();
    void start();
    void run();
    static bool isRunning();
    static void simSigintHandler(int sig);
    void processSimCommand(SimCommand sc);

private:
    void placeObjectsFromConf();

    essentials::SystemConfig* sc;
    static bool running;
    bool headless;
    World* world;
    GUI* gui;
    communication::Communication* communication;

    essentials::IDManager* idManager;
    std::thread* mainThread;

    mutable std::recursive_mutex commandMutex;
    std::queue<SimCommand> commandQueue;
    std::vector<commands::CommandHandler*> communicationHandlers;
};
} // namespace srgsim
