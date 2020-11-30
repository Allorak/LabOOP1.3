#include "instruction.h"
#include "physicVector.h"

Instruction::Instruction(double velocityX,double velocityY,double initialTime)
{
    _velocity = PhysicVector{velocityX,velocityY};
    _initialTime = initialTime;
}
Instruction::Instruction(const Instruction& instruction)
{
    _velocity = instruction._velocity;
}

Instruction::Instruction(const PhysicVector& velocity,double initialTime)
{
    _velocity = velocity;
    _initialTime = initialTime;
}


