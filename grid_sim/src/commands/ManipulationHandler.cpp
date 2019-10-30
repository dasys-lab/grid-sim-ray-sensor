#include "srgsim/commands/ManipulationHandler.h"

#include "srgsim/world/ServiceRobot.h"

namespace srgsim
{
namespace commands
{

bool ManipulationHandler::handle(SimCommand sc)
{
    switch(sc.action) {
        case SimCommand::OPEN:
        case SimCommand::CLOSE:
        case SimCommand::PUTDOWN:
        case SimCommand::PICKUP: {
            ServiceRobot *robot = this->world->getRobot(sc.senderID);
            if (robot) {
                robot->executeAction(sc, this->world);
            }
            return true;
        }
        default:
            return false;
    }
}
} // namespace communication
} // namespace srgsim