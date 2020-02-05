#include "srgsim/containers/FloatCoordinate.h"
#include <iostream>

namespace srgsim {
    FloatCoordinate::FloatCoordinate(float x, float y) {
        this->x = x;
        this->y = y;
    }

    FloatCoordinate::FloatCoordinate(const srgsim::FloatCoordinate &coordinate) {
        this->x = coordinate.x;
        this->y = coordinate.y;
    }

    bool operator==(FloatCoordinate const &first, FloatCoordinate const &second) {
        return first.x == second.x && first.y == second.y;
    }

    bool operator!=(FloatCoordinate const &first, FloatCoordinate const &second) {
        return !(first == second);
    }

    bool operator<(FloatCoordinate const &first, FloatCoordinate const &second) {
        if (first.x < second.x) {
            return true;
        } else if (first.x > second.x) {
            return false;
        } else {
            return (first.y < second.y);
        }
    }

} // namespace srgsim