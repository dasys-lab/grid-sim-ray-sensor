#pragma once

#include "SimCommand.h"
#include "SimPerceptions.h"

#include <capnzero/CapnZero.h>

namespace essentials {
    class IDManager;
}

namespace srgsim {
    class ContainerUtils {
    public:
        static SimCommand toSimCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
        static void toMsg(SimCommand simCommand, ::capnp::MallocMessageBuilder& builder);

        static SimPerceptions toSimPerceptions(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
        static void toMsg(SimPerceptions simPerceptions, ::capnp::MallocMessageBuilder& builder);

    private:
        ContainerUtils() = delete;
    };
}