#include "velocityinstruction.h"


VelocityInstruction::VelocityInstruction(const VelocityInstruction& instruction):
    Instruction(instruction) {}

VelocityInstruction::VelocityInstruction(double velocityX,double velocityY,double initialTime):
    Instruction(velocityX,velocityY,initialTime) {}

VelocityInstruction::VelocityInstruction(const PhysicVector& velocity,double initialTime):
    Instruction(velocity,initialTime) {}
