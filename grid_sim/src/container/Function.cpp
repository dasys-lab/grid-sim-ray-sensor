//
// Created by kopytg on 04.12.2019.
//

#include <iostream>
#include <srgsim/containers/Function.h>
namespace srgsim {
    Function::Function(Coordinate start, Coordinate end) {
        float a = (end.y - start.y);
        float b = (end.x - start.x);
        m = a / b;

        std::cout << "m = " << m << " a = " << a << " b = " << b << std::endl;
        b = (end.x * start.y - start.x * end.y) / (end.x - start.x);
    }

    float Function::calculateY(int x) {
        auto a = (float) x;
        return m * a + b;
    }

    float Function::calculateX(int y) {
        auto a = (float) y;
        return (a - b) / m;
    }
}



