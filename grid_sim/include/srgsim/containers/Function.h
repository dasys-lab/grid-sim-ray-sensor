#pragma once

#include <iostream>
#include "Coordinate.h"

namespace srgsim
{

class Function
{
public:
    Function(Coordinate& start, Coordinate& end);
    virtual ~Function() = default;

    float m;
    float b;

    float calculateX(int y);
    float calculateY(int x);

    friend std::ostream& operator<<(std::ostream& os, const srgsim::Function& obj)
    {
        os << obj.m << "x + " << obj.b;
        return os;
    }
};
} // namespace srgsim