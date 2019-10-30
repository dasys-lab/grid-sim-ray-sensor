#pragma once

#include "srgsim/containers/SimPerceptions.h"

#include <capnp/serialize-packed.h>
#include <string>
#include <vector>

namespace essentials
{
class IDManager;
class SystemConfig;
} // namespace essentials

namespace capnzero
{
class Subscriber;
class Publisher;
} // namespace capnzero

namespace srgsim
{
class Simulator;
namespace communication
{
class Communication
{
public:
    Communication(essentials::IDManager* idManager, Simulator* simulator);
    ~Communication();

    void sendSimPerceptions(SimPerceptions sp);

private:
    void onSimCommand(::capnp::FlatArrayMessageReader& msg);

    essentials::SystemConfig* sc;
    void* ctx;
    std::string simCommandTopic;
    std::string simPerceptionsTopic;
    std::string address;
    capnzero::Subscriber* simCommandSub;
    capnzero::Publisher* simPerceptionsPub;

    essentials::IDManager* idManager;
    Simulator* simulator;
};
} // namespace communication
} // namespace srgsim
