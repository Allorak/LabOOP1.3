#include <cmath>
#include "physicVector.h"
PhysicVector::PhysicVector(double X,double Y)
{
    _x=X;
    _y=Y;
}

PhysicVector::PhysicVector(const PhysicVector& vector)
{
    _x = vector._x;
    _y = vector._y;
}

double PhysicVector::GetVectorLength() const
{
    return sqrt(pow(_x,2)+pow(_y,2));
}

