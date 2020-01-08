#include "srgsim/containers/Function.h"
#include <iostream>

namespace srgsim
{
Function:Function(Coordinate& start, Coordinate& end){
    this->m = (end->y - start->y) / (end->x - start->x));
    this->b = (end->x * start->y - start->x * end->y) / (end->x - start->x);
}

float Function:calculateX(float y){
    return (y-this->b) / this->m;
}

float Function:calculateY(float x){
    return this->m*x + this->b
}

} // namespace srgsim