#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "physicVector.h"
#include <ctime>
#include <cmath>
#include <QString>
enum class Direction
{
    LEFT,
    RIGHT
};

class Instruction
{
public:
    Instruction() = default;
    Instruction(const Instruction& instruction);
    Instruction(double velocityX,double velocityY,double initialTime = round(clock()/100.0)/10.0);
    Instruction(const PhysicVector& velocity,double initialTime = round(clock()/100.0)/10.0);
    ~Instruction() = default;
    double GetTime() const {return  _initialTime;};
    const PhysicVector& GetVelocity() const {return  _velocity;};
    virtual QString GetType() const = 0;
    virtual Direction GetDirection() const = 0;
    virtual double GetTurnRadius() const = 0;
protected:
    PhysicVector _velocity {0,0};
private:
    double _initialTime ;
};


#endif // INSTRUCTION_H
