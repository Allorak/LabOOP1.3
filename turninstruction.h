#ifndef TURNINSTRUCTION_H
#define TURNINSTRUCTION_H
#include "instruction.h"

class TurnInstruction : public Instruction
{
public:
    TurnInstruction() = default;
    TurnInstruction(const TurnInstruction& instruction);
    TurnInstruction(Direction direction,double turnRadius, double velocityX,double velocityY,double initialTime = round(clock()/100.0)/10.0);
    TurnInstruction(Direction direction,double turnRadius, const PhysicVector& velocity,double initialTime = round(clock()/100.0)/10.0);
    ~TurnInstruction() = default;
    friend bool operator==(const TurnInstruction &i1,const TurnInstruction &i2)
    {
        return i1._velocity==i2._velocity &&
               i1._direction == i2._direction &&
               i1._turnRadius == i2._turnRadius;
    };
    friend bool operator!=(const TurnInstruction &i1,const TurnInstruction &i2)
    {return !(i1==i2);};
    Direction GetDirection() const override {return  _direction;};
    double GetTurnRadius() const override {return _turnRadius;};
    QString GetType() const override {return "Turn";} ;
private:
    Direction _direction;
    double _turnRadius;
};

#endif // TURNINSTRUCTION_H
