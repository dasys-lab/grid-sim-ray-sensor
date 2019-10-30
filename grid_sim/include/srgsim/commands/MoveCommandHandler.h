#pragma once

#include "CommandHandler.h"

namespace srgsim
{
namespace commands
{
class MoveCommandHandler : public CommandHandler
{

public:
    MoveCommandHandler(World* world): CommandHandler(world) {}
    ~MoveCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
