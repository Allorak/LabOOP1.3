#ifndef VELOCITYINSTRUCTION_H
#define VELOCITYINSTRUCTION_H
#include "instruction.h"

class VelocityInstruction : public Instruction
{
public:
    VelocityInstruction() = default;
    VelocityInstruction(const VelocityInstruction& instruction);
    VelocityInstruction(double velocityX,double velocityY,double initialTime = round(clock()/100.0)/10.0);
    VelocityInstruction(const PhysicVector& velocity,double initialTime = round(clock()/100.0)/10.0);
    ~VelocityInstruction() = default;
    friend bool operator==(const VelocityInstruction &i1,const VelocityInstruction &i2)
    {
        return i1._velocity==i2._velocity;
    };
    friend bool operator!=(const VelocityInstruction &i1,const VelocityInstruction &i2)
    {return !(i1==i2);};
    QString GetType() const override {return "Velocity";};
    Direction GetDirection() const override {};
    double GetTurnRadius() const override {};
};

#endif // VELOCITYINSTRUCTION_H
