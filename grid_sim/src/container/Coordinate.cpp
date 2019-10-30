#include "srgsim/containers/Coordinate.h"
#include <iostream>

namespace srgsim
{
Coordinate::Coordinate(int32_t x, int32_t y)
{
    this->x = x;
    this->y = y;
}

Coordinate::Coordinate(const srgsim::Coordinate& coordinate)
{
    this->x = coordinate.x;
    this->y = coordinate.y;
}

bool operator==(Coordinate const& first, Coordinate const& second)
{
    return first.x == second.x && first.y == second.y;
}

bool operator!=(Coordinate const& first, Coordinate const& second)
{
    return !(first == second);
}

bool operator<(Coordinate const& first, Coordinate const& second)
{
    if (first.x < second.x) {
        return true;
    } else if (first.x > second.x) {
        return false;
    } else {
        return (first.y < second.y);
    }
}

} // namespace srgsim