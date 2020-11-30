#ifndef PHYSICVECTOR_H
#define PHYSICVECTOR_H

class PhysicVector
{
public:
    PhysicVector() = default;
    PhysicVector(const PhysicVector& vector);
    PhysicVector(double X,double Y);
    friend bool operator==(const PhysicVector &pv1,const PhysicVector &pv2)
    {return (pv1._x==pv2._x && pv1._y==pv2._y);};
    friend bool operator!=(const PhysicVector &pv1,const PhysicVector &pv2)
    {return !(pv1==pv2);};
    friend PhysicVector operator+(const PhysicVector &pv1,const PhysicVector &pv2)
    {
        return PhysicVector{pv1._x+pv2._x,pv1._y+pv2._y};
    };
    friend void operator+=(PhysicVector &pv1,const PhysicVector &pv2)
    { PhysicVector pv =pv1+pv2;
        pv1._x=pv._x;
        pv1._y=pv._y;
    };
    double GetVectorLength() const;
    double GetX()const {return _x;};
    double GetY()const {return _y;};
protected:
private:
    double _x{0};
    double _y{0};
};
#endif // PHYSICVECTOR_H
