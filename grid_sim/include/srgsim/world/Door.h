#pragma once

#include "Object.h"

namespace srgsim
{
class Door : public Object
{
public:
    Door(essentials::IdentifierConstPtr id, State state = State::Closed);
    bool isOpen();
};
} // namespace srgsim
