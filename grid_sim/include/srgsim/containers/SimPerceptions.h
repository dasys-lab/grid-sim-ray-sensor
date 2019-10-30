#pragma once

#include "CellPerceptions.h"

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>

namespace srgsim
{
    struct SimPerceptions
    {
        essentials::IdentifierConstPtr receiverID;
        std::vector<CellPerceptions> cellPerceptions;
    };
} // namespace srgsim