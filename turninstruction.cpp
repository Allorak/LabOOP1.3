#include "turninstruction.h"

TurnInstruction::TurnInstruction(const TurnInstruction& instruction):
    Instruction(instruction),_direction{instruction._direction},_turnRadius{instruction._turnRadius} {}

TurnInstruction::TurnInstruction(Direction direction,double turnRadius, double velocityX,double velocityY,double initialTime):
    Instruction(velocityX,velocityY,initialTime),_direction{direction},_turnRadius{turnRadius} {}

TurnInstruction::TurnInstruction(Direction direction,double turnRadius, const PhysicVector& velocity,double initialTime):
    Instruction(velocity,initialTime),_direction{direction},_turnRadius{turnRadius} {}

