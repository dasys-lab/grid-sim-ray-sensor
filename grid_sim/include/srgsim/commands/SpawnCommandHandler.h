#pragma once

#include "CommandHandler.h"

namespace srgsim
{
namespace commands
{
class SpawnCommandHandler : public CommandHandler
{

public:
    SpawnCommandHandler(World* world): CommandHandler(world) {}
    ~SpawnCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
