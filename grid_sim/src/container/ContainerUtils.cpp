#include "srgsim/containers/ContainerUtils.h"

#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>
#include <srgsim/msgs/SimCommandMsg.capnp.h>
#include <srgsim/msgs/SimPerceptionsMsg.capnp.h>

namespace srgsim
{
SimCommand ContainerUtils::toSimCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    SimCommand sc;
    srgsim::SimCommandMsg::Reader reader = msg.getRoot<srgsim::SimCommandMsg>();
    sc.senderID = idManager->getIDFromBytes(
            reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), reader.getSenderID().getType());
    sc.objectID = idManager->getIDFromBytes(
            reader.getObjectID().getValue().asBytes().begin(), reader.getObjectID().getValue().size(), reader.getObjectID().getType());

    switch (reader.getAction()) {
    case srgsim::SimCommandMsg::Action::SPAWN:
        sc.action = SimCommand::SPAWN;
        break;
    case srgsim::SimCommandMsg::Action::CLOSE:
        sc.action = SimCommand::CLOSE;
        break;
    case srgsim::SimCommandMsg::Action::OPEN:
        sc.action = SimCommand::OPEN;
        break;
    case srgsim::SimCommandMsg::Action::PICKUP:
        sc.action = SimCommand::PICKUP;
        break;
    case srgsim::SimCommandMsg::Action::PUTDOWN:
        sc.action = SimCommand::PUTDOWN;
        break;
    case srgsim::SimCommandMsg::Action::GODOWN:
        sc.action = SimCommand::GODOWN;
        break;
    case srgsim::SimCommandMsg::Action::GOLEFT:
        sc.action = SimCommand::GOLEFT;
        break;
    case srgsim::SimCommandMsg::Action::GORIGHT:
        sc.action = SimCommand::GORIGHT;
        break;
    case srgsim::SimCommandMsg::Action::GOUP:
        sc.action = SimCommand::GOUP;
        break;
    default:
        std::cerr << "srgsim::ContainerUtils::toSimCommand(): Unknown action!" << std::endl;
    }

    sc.x = reader.getX();
    sc.y = reader.getY();

    return sc;
}

void ContainerUtils::toMsg(srgsim::SimCommand sc, ::capnp::MallocMessageBuilder& builder)
{
    SimCommandMsg::Builder msg = builder.getRoot<SimCommandMsg>();

    capnzero::ID::Builder senderID = msg.initSenderID();
    senderID.setValue(kj::arrayPtr(sc.senderID->getRaw(), (unsigned int) sc.senderID->getSize()));
    senderID.setType(sc.senderID->getType());

    capnzero::ID::Builder objectID = msg.initObjectID();
    objectID.setValue(kj::arrayPtr(sc.objectID->getRaw(), (unsigned int) sc.objectID->getSize()));
    objectID.setType(sc.objectID->getType());

    switch (sc.action) {
    case SimCommand::SPAWN:
        msg.setAction(srgsim::SimCommandMsg::Action::SPAWN);
        break;
    case SimCommand::CLOSE:
        msg.setAction(srgsim::SimCommandMsg::Action::CLOSE);
        break;
    case SimCommand::OPEN:
        msg.setAction(srgsim::SimCommandMsg::Action::OPEN);
        break;
    case SimCommand::PICKUP:
        msg.setAction(srgsim::SimCommandMsg::Action::PICKUP);
        break;
    case SimCommand::PUTDOWN:
        msg.setAction(srgsim::SimCommandMsg::Action::PUTDOWN);
        break;
    case SimCommand::GODOWN:
        msg.setAction(srgsim::SimCommandMsg::Action::GODOWN);
        break;
    case SimCommand::GOLEFT:
        msg.setAction(srgsim::SimCommandMsg::Action::GOLEFT);
        break;
    case SimCommand::GORIGHT:
        msg.setAction(srgsim::SimCommandMsg::Action::GORIGHT);
        break;
    case SimCommand::GOUP:
        msg.setAction(srgsim::SimCommandMsg::Action::GOUP);
        break;
    default:
        std::cerr << "srgsim::ContainerUtils::toMsg(): Unknown action!" << std::endl;
    }

    msg.setX(sc.x);
    msg.setY(sc.y);
}

SimPerceptions ContainerUtils::toSimPerceptions(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    SimPerceptions sps;
    srgsim::SimPerceptionsMsg::Reader reader = msg.getRoot<srgsim::SimPerceptionsMsg>();
    sps.receiverID = idManager->getIDFromBytes(
            reader.getReceiverID().getValue().asBytes().begin(), reader.getReceiverID().getValue().size(), reader.getReceiverID().getType());
    for (srgsim::SimPerceptionsMsg::CellPerceptions::Reader cellPerceptionsMsg : reader.getCellPerceptions()) {
        srgsim::CellPerceptions cellPerceptions;
        cellPerceptions.x = cellPerceptionsMsg.getX();
        cellPerceptions.y = cellPerceptionsMsg.getY();

        for (srgsim::SimPerceptionsMsg::Perception::Reader perceptionMsg : cellPerceptionsMsg.getPerceptions()) {
            srgsim::Perception perception;
            perception.objectID = idManager->getIDFromBytes(perceptionMsg.getObjectID().getValue().asBytes().begin(),
                    perceptionMsg.getObjectID().getValue().size(), perceptionMsg.getObjectID().getType());
            perception.x = perceptionMsg.getX();
            perception.y = perceptionMsg.getY();
            switch (perceptionMsg.getType()) {
            case srgsim::SimPerceptionsMsg::Perception::Type::ROBOT:
                perception.type = Type::Robot;
                break;
            case srgsim::SimPerceptionsMsg::Perception::Type::DOOR:
                perception.type = Type::Door;
                break;
            case srgsim::SimPerceptionsMsg::Perception::Type::CUPRED:
                perception.type = Type::CupRed;
                break;
            case srgsim::SimPerceptionsMsg::Perception::Type::CUPBLUE:
                perception.type = Type::CupBlue;
                break;
            case srgsim::SimPerceptionsMsg::Perception::Type::CUPYELLOW:
                perception.type = Type::CupYellow;
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toSimPerceptions(): Unknown object type in capnp message found!" << std::endl;
                break;
            }
            switch (perceptionMsg.getState()) {
            case srgsim::SimPerceptionsMsg::Perception::State::OPEN:
                perception.state = State::Open;
                break;
            case srgsim::SimPerceptionsMsg::Perception::State::CLOSED:
                perception.state = State::Closed;
                break;
            case srgsim::SimPerceptionsMsg::Perception::State::UNDEFINED:
                perception.state = State::Undefined;
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toSimPerceptions(): Unknown object type in capnp message found!" << std::endl;
                break;
            }
            cellPerceptions.perceptions.push_back(perception);
        }
        sps.cellPerceptions.push_back(cellPerceptions);
    }
    return sps;
}

void ContainerUtils::toMsg(srgsim::SimPerceptions sp, ::capnp::MallocMessageBuilder& builder)
{
    SimPerceptionsMsg::Builder msg = builder.getRoot<SimPerceptionsMsg>();

    capnzero::ID::Builder receiverID = msg.initReceiverID();
    receiverID.setValue(kj::arrayPtr(sp.receiverID->getRaw(), (unsigned int) sp.receiverID->getSize()));
    receiverID.setType(sp.receiverID->getType());

    ::capnp::List<::srgsim::SimPerceptionsMsg::CellPerceptions>::Builder cellPerceptionsBuilder = msg.initCellPerceptions(sp.cellPerceptions.size());
    for (unsigned int i = 0; i < sp.cellPerceptions.size(); i++) {
        srgsim::SimPerceptionsMsg::CellPerceptions::Builder cpBuilder = cellPerceptionsBuilder[i];
        cpBuilder.setX(sp.cellPerceptions[i].x);
        cpBuilder.setY(sp.cellPerceptions[i].y);

        ::capnp::List<::srgsim::SimPerceptionsMsg::Perception>::Builder perceptionsBuilder = cpBuilder.initPerceptions(sp.cellPerceptions[i].perceptions.size());
        for (unsigned int j = 0; j < sp.cellPerceptions[i].perceptions.size(); j++) {
            srgsim::SimPerceptionsMsg::Perception::Builder pBuilder = perceptionsBuilder[j];
            pBuilder.setY(sp.cellPerceptions[i].perceptions[j].y);
            pBuilder.setX(sp.cellPerceptions[i].perceptions[j].x);
            switch (sp.cellPerceptions[i].perceptions[j].type) {
            case Type::Robot:
                pBuilder.setType(srgsim::SimPerceptionsMsg::Perception::Type::ROBOT);
                break;
            case Type::CupRed:
                pBuilder.setType(srgsim::SimPerceptionsMsg::Perception::Type::CUPRED);
                break;
            case Type::CupBlue:
                pBuilder.setType(srgsim::SimPerceptionsMsg::Perception::Type::CUPBLUE);
                break;
            case Type::CupYellow:
                pBuilder.setType(srgsim::SimPerceptionsMsg::Perception::Type::CUPYELLOW);
                break;
            case Type::Door:
                pBuilder.setType(srgsim::SimPerceptionsMsg::Perception::Type::DOOR);
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toMsg(): Unknown object type perceived: " << static_cast<int>(sp.cellPerceptions[i].perceptions[j].type) << "!" << std::endl;
                break;
            }
            switch (sp.cellPerceptions[i].perceptions[j].state) {
            case State::Open:
                pBuilder.setState(srgsim::SimPerceptionsMsg::Perception::State::OPEN);
                break;
            case State::Closed:
                pBuilder.setState(srgsim::SimPerceptionsMsg::Perception::State::CLOSED);
                break;
            case State::Undefined:
                pBuilder.setState(srgsim::SimPerceptionsMsg::Perception::State::UNDEFINED);
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toMsg(): Unknown object state perceived: " << static_cast<int>(sp.cellPerceptions[i].perceptions[j].state) << "!" << std::endl;
                break;
            }
            capnzero::ID::Builder objectID = pBuilder.initObjectID();
            objectID.setType(sp.cellPerceptions[i].perceptions[j].objectID->getType());
            objectID.setValue(::capnp::Data::Reader(sp.cellPerceptions[i].perceptions[j].objectID->getRaw(), sp.cellPerceptions[i].perceptions[j].objectID->getSize()));
        }
    }
}
} // namespace srgsim