#pragma once

#include "Perception.h"

namespace srgsim
{
struct CellPerceptions
{
    uint32_t x;
    uint32_t y;
    std::vector<Perception> perceptions;

    friend std::ostream& operator<<(std::ostream& os, const srgsim::CellPerceptions& obj)
    {
//        os << "CellPerceptions: X: " << obj.x << " Y: " << obj.y << std::endl;
        for (Perception p : obj.perceptions) {
            os << p;
        }
        return os;
    }
};
} // namespace srgsim