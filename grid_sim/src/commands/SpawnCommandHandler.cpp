#include "srgsim/commands/SpawnCommandHandler.h"

#include "srgsim/Simulator.h"

#include <capnzero/ID.capnp.h>
#include <essentials/IDManager.h>

namespace srgsim
{
namespace commands
{

bool SpawnCommandHandler::handle(SimCommand sc)
{
    if (sc.action == SimCommand::SPAWN) {
        this->world->spawnRobot(sc.senderID);
        return true;
    } else {
        return false;
    }
}
} // namespace communication
} // namespace srgsim