#pragma once

#include "srgsim/SRGEnums.h"

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>

namespace srgsim
{
    struct Perception
    {
        Type type;
        State state;
        essentials::IdentifierConstPtr objectID;
        uint32_t x;
        uint32_t y;

        friend std::ostream& operator<<(std::ostream& os, const srgsim::Perception& obj)
        {
            os << "Perception - Type: " << static_cast<int>(obj.type) << " State: " << static_cast<int>(obj.state) << " ID: " << (*obj.objectID) << " At: (" << obj.x << ", " << obj.y << ")" << std::endl;
            return os;
        }
    };
} // namespace srgsim