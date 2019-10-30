#pragma once

#include <stdint.h>
#include <iostream>

namespace srgsim
{

class Coordinate
{
public:
    Coordinate(int32_t x, int32_t y);
    virtual ~Coordinate() = default;
    Coordinate(const Coordinate& coordinate);

    int32_t x;
    int32_t y;

    friend std::ostream& operator<<(std::ostream& os, const srgsim::Coordinate& obj)
    {
        os << "(" << obj.x << ", " << obj.y << ")";
        return os;
    }
};

bool operator==(Coordinate const& first, Coordinate const& second);
bool operator!=(Coordinate const& first, Coordinate const& second);
bool operator<(Coordinate const& first, Coordinate const& second);
} // namespace srgsim