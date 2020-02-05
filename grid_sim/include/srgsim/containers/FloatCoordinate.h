#pragma once

#include <iostream>

namespace srgsim
{

class FloatCoordinate
{
public:
    FloatCoordinate(float x, float y);
    virtual ~FloatCoordinate() = default;
    FloatCoordinate(const FloatCoordinate& coordinate);

    float x;
    float y;

    friend std::ostream& operator<<(std::ostream& os, const srgsim::FloatCoordinate& obj)
    {
        os << "(" << obj.x << ", " << obj.y << ")";
        return os;
    }
};

bool operator==(FloatCoordinate const& first, FloatCoordinate const& second);
bool operator!=(FloatCoordinate const& first, FloatCoordinate const& second);
bool operator<(FloatCoordinate const& first, FloatCoordinate const& second);
} // namespace srgsim