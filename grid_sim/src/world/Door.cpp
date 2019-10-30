#include "srgsim/world/Door.h"

#include <essentials/IdentifierConstPtr.h>
#include <srgsim/SRGEnums.h>

namespace srgsim
{
Door::Door(essentials::IdentifierConstPtr id, State state)
        : Object(Type::Door, id, state)
{
}

bool Door::isOpen() {
    return this->state == State::Open;
};

} // namespace srgsim
