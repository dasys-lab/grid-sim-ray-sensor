#include "srgsim/communication/Communication.h"

#include "srgsim/Simulator.h"
#include "srgsim/msgs/SimCommandMsg.capnp.h"
#include "srgsim/containers/ContainerUtils.h"

#include <capnzero/Subscriber.h>
#include <essentials/IDManager.h>
#include <SystemConfig.h>

#include <vector>

namespace srgsim
{
namespace communication
{

Communication::Communication(essentials::IDManager* idManager, Simulator* simulator)
        : simulator(simulator)
        , idManager(idManager)
{

    this->sc = essentials::SystemConfig::getInstance();
    this->ctx = zmq_ctx_new();
    this->address = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.address", NULL);
    this->simCommandTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.cmdTopic", NULL);

    this->simCommandSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->simCommandSub->setTopic(this->simCommandTopic);
    this->simCommandSub->addAddress(this->address);
    this->simCommandSub->subscribe(&Communication::onSimCommand, &(*this));

    this->simPerceptionsTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.perceptionsTopic", NULL);
    this->simPerceptionsPub = new capnzero::Publisher(this->ctx, capnzero::Protocol::UDP);
    this->simPerceptionsPub->setDefaultTopic(simPerceptionsTopic);
    this->simPerceptionsPub->addAddress(this->address);
}

Communication::~Communication()
{
    delete this->simCommandSub;
    zmq_ctx_term(this->ctx);
}

void Communication::onSimCommand(::capnp::FlatArrayMessageReader& msg)
{
    this->simulator->processSimCommand(ContainerUtils::toSimCommand(msg, this->idManager));
}

void Communication::sendSimPerceptions(srgsim::SimPerceptions sp)
{
    ::capnp::MallocMessageBuilder msgBuilder;
    ContainerUtils::toMsg(sp, msgBuilder);
    this->simPerceptionsPub->send(msgBuilder);
}
} // namespace communication
} // namespace srgsim
