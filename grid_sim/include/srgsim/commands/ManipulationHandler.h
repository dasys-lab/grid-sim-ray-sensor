#pragma once

#include "CommandHandler.h"

namespace srgsim
{
namespace commands
{
class ManipulationHandler : public CommandHandler
{

public:
    ManipulationHandler(World* world): CommandHandler(world) {}
    ~ManipulationHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
