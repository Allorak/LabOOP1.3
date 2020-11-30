#ifndef CAR_H
#define CAR_H
#include "physicVector.h"
#include "instruction.h"
#include "velocityinstruction.h"
#include "turninstruction.h"
#include <string>
#include "queue.h"
#include <QtWidgets>
#include <memory>

class Car
{
public:
    Car() = default;
    Car(const Car& car);
    Car(double positionX, double positionY,double velocityX=0,double velocityY=0);
    const PhysicVector& GetPosition() const {return _position;};
    const PhysicVector& GetVelocity() const {return _velocity;};
    void AddToQueue(double velocityX, double velocityY);
    void AddToQueue(Direction direction, double turnRadius, double velocityX, double velocityY);
    void SetQueue(QString path);
    void CountPosition(QLabel* label);
    void ClearAllInstructions();
    void SaveQueueToFile(QString path);
    void PrintToTable(QTableWidget* table, int param);
    QString DirToStr(Direction dir) const
        {
            switch(dir)
            {
                case Direction::LEFT:
                    return "LEFT";
                    break;
                case Direction::RIGHT:
                    return "RIGHT";
                    break;
                default:
                    throw std::invalid_argument("No such direction");
                    break;
            }
        };
    Direction StrToDir(QString str) const
    {
        if(str=="LEFT")
            return Direction::LEFT;
        if(str=="RIGHT")
            return Direction::RIGHT;
        throw std::invalid_argument("No such direction");
        }
protected:
private:
    void Move(double time, std::shared_ptr<Instruction> instruction);
    Queue<std::shared_ptr<Instruction> > _currentInstructions;
    Queue<std::shared_ptr<Instruction> > _allInstructions;
    PhysicVector _position{0,0};
    PhysicVector _velocity{0,0};
    double startTime {0};
};

#endif // CAR_H
