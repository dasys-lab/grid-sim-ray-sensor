#pragma once

#include <iostream>
#include "Coordinate.h"
#include "FloatCoordinate.h"

namespace srgsim
{

class Function
{
public:
    Function(FloatCoordinate& start, FloatCoordinate& end);
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