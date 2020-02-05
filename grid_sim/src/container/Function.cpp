//
// Created by kopytg on 04.12.2019.
//

#include <iostream>
#include <srgsim/containers/Function.h>
#include <math.h>

namespace srgsim {
    Function::Function(Coordinate& start, Coordinate& end) {
        float a = (end.y - start.y);
        float b = (end.x - start.x);
        m = a / b;

        if(isinf(m)){
            this->b = 0;
            std::cout << "m = " << m << " a = " << a << " b = " << b << std::endl;
            return;
        }

        std::cout << "m = " << m << " a = " << a << " b = " << b << std::endl;
        this->b = (end.x * start.y - start.x * end.y) / (end.x - start.x);
    }

    float Function::calculateY(int x) {
        float a = (float) x;
        if(m == 0){
            return b;
        }
        return m * a + b;
    }

    float Function::calculateX(int y) {
        float a = (float) y;
        if(isinf(m)){
            return m;
        }
        return (a - b) / m;
    }
}



