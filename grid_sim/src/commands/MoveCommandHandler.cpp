#include "srgsim/commands/MoveCommandHandler.h"

#include "srgsim/Simulator.h"

#include <essentials/IDManager.h>

namespace srgsim
{
namespace commands
{

bool MoveCommandHandler::handle(SimCommand sc)
{
    switch (sc.action) {
    case SimCommand::GOLEFT:
        this->world->moveObject(sc.senderID, Direction::Left);
        return true;
    case SimCommand::GOUP:
        this->world->moveObject(sc.senderID, Direction::Up);
        return true;
    case SimCommand::GORIGHT:
        this->world->moveObject(sc.senderID, Direction::Right);
        return true;
    case SimCommand::GODOWN:
        this->world->moveObject(sc.senderID, Direction::Down);
        return true;
    default:
        return false;
    }
}
} // namespace communication
} // namespace srgsim