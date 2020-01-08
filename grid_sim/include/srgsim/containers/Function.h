#pragma once

#include <iostream>

namespace srgsim
{

class Function
{
public:
    Function(Coordinate& start, Coordinate& end);
    virtual ~Function() = default;

    float m;
    float b;

    float calculateX(float y);
    float calculateY(float x);

    friend std::ostream& operator<<(std::ostream& os, const srgsim::Function& obj)
    {
        os << obj.m << "x + " << obj.b;
        return os;
    }
};
} // namespace srgsim