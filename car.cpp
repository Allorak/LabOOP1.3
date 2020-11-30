#include <iostream>
#include <cmath>
#include "car.h"
#include "physicVector.h"
#include "queue.h"
#include "instruction.h"
#include "velocityinstruction.h"
#include "turninstruction.h"
#define _USE_MATH_DEFINES

Car::Car(double positionX, double positionY, double velocityX, double velocityY)
{
   _position = PhysicVector {positionX,positionY};
   _velocity = PhysicVector {velocityX,velocityY};
}

Car::Car(const Car& car)
{
    _position = car._position;
    _velocity = car._velocity;
}

void Car::Move(double time, std::shared_ptr<Instruction> instruction)
{
    float deltaX,deltaY;
    //set velocity equal to the instruction velocity
    float vX = instruction->GetVelocity().GetX();
    float vY = instruction->GetVelocity().GetY();
    auto instType = instruction->GetType();
    if(instruction->GetType() == "Velocity")
    {
     //calculate new position for the car if it moves straight forward
        deltaX = vX*time;
        deltaY = vY*time;
        PhysicVector deltaPosition{deltaX,deltaY};
        _position+=deltaPosition;
        _velocity = PhysicVector{vX,vY};
    }
    else
    {
     //calculate new position for the car if it moves circularly
        double turnRadius = instruction->GetTurnRadius();
        Direction direction = instruction->GetDirection();
        double linearVelocity = instruction->GetVelocity().GetVectorLength();
        double turningArcLength = linearVelocity*time;
        double turningAngle = turningArcLength/turnRadius;
        while(turningAngle>=2*M_PI)
            turningAngle-=2*M_PI;
        if(direction == Direction::RIGHT)
            deltaX = turnRadius*(1-cos(turningAngle));
        else
            deltaX = -turnRadius*(1-cos(turningAngle));
        deltaY = turnRadius*sin(turningAngle);
        PhysicVector deltaPosition{deltaX,deltaY};
        _position+=deltaPosition;

        //calculate new car velocity vector
        float newVelocityX,newVelocityY;
        newVelocityX = vX*cos(turningAngle)+vY*sin(turningAngle);
        newVelocityY = -vX*sin(turningAngle)+vY*cos(turningAngle);
        _velocity = PhysicVector {newVelocityX,newVelocityY};
    }
}

void Car::AddToQueue(double velocityX,double velocityY)
{
    _currentInstructions.Enqueue(std::make_shared<VelocityInstruction>(velocityX,velocityY,
                                                                       startTime+round(clock()/100.0)/10.0));
    _allInstructions.Enqueue(std::make_shared<VelocityInstruction>(velocityX,velocityY,
                                                                   startTime+round(clock()/100.0)/10.0));
}

void Car::AddToQueue(Direction direction, double turnRadius, double velocityX,double velocityY)
{
    _currentInstructions.Enqueue(std::make_shared<TurnInstruction>(direction, turnRadius,velocityX,velocityY,
                                                                   startTime+round(clock()/100.0)/10.0));
    _allInstructions.Enqueue(std::make_shared<TurnInstruction>(direction, turnRadius,velocityX,velocityY,
                                                               startTime+round(clock()/100.0)/10.0));
}

void Car::CountPosition(QLabel* label)
{
    double startOfMovingTime = startTime+round(clock()/100.0)/10.0;
    label->setNum(startOfMovingTime);
    while(_currentInstructions.Count()>1)
    {
        double time = _currentInstructions.Head()->GetTime();
        _currentInstructions.Dequeue();
        Move(_currentInstructions.Head()->GetTime()-time,_currentInstructions.Head());
    }
    if(_currentInstructions.Count()==1)
    {
        double time = _currentInstructions.Head()->GetTime();
        Move(startOfMovingTime-time,_currentInstructions.Head());
    }
}

void Car::SetQueue(QString path)
{
    QFile json(path);
    if(json.open(QFile::ReadOnly))
    {
        _currentInstructions.Erase();
        _allInstructions.Erase();
        QJsonDocument dataDocument = QJsonDocument().fromJson(json.readAll());
        QJsonArray dataArray = dataDocument.array();
        while(!dataArray.empty())
        {
            QJsonObject object = dataArray.first().toObject();
            double velocityX = object["velocity"].toObject()["x"].toDouble();
            double velocityY = object["velocity"].toObject()["y"].toDouble();
            double time = object["time"].toDouble();
            if(object["instruction type"].toObject()["type"]=="Velocity")
            {
                _currentInstructions.Enqueue(std::make_shared<VelocityInstruction>(velocityX,velocityY,time));
                _allInstructions.Enqueue(std::make_shared<VelocityInstruction>(velocityX,velocityY,time));
            }
            else
            {
                Direction direction = StrToDir(object["instruction type"].toObject()["direction"].toString());
                double turnRadius = object["instruction type"].toObject()["turn radius"].toDouble();
                _currentInstructions.Enqueue(std::make_shared<TurnInstruction>(direction, turnRadius,velocityX,velocityY,time));
                _allInstructions.Enqueue(std::make_shared<TurnInstruction>(direction, turnRadius,velocityX,velocityY,time));
            }
            startTime = time;
            dataArray.pop_front();
        }
    }
    else
    {
        std::cout<<"There is no file with this path"<<std::endl;
    }
}

void Car::SaveQueueToFile(QString path)
{
    if(_allInstructions.Count()!=0)
        {
            QJsonArray dataArray;
            auto _allInstructionsCopy = _allInstructions;
            while(_allInstructionsCopy.Count()>0)
            {
                auto iterator = _allInstructionsCopy.Head();
                QJsonObject object;
                object.insert("time",iterator->GetTime());
                QJsonObject velocityObject;
                velocityObject.insert("x",iterator->GetVelocity().GetX());
                velocityObject.insert("y",iterator->GetVelocity().GetY());
                object.insert("velocity",velocityObject);
                QJsonObject typeObject;
                typeObject.insert("type",iterator->GetType());
                if(iterator->GetType()=="Turn")
                {
                    typeObject.insert("direction",DirToStr(iterator->GetDirection()));
                    typeObject.insert("turn radius",iterator->GetTurnRadius());
                }
                object.insert("instruction type",typeObject);
                dataArray.append(object);
                _allInstructionsCopy.Dequeue();
            }
            QJsonDocument dataDocument;
            dataDocument.setArray(dataArray);
            QFile json(path);
            json.open(QFile::WriteOnly);
            json.write(dataDocument.toJson());
        }
}

void Car::ClearAllInstructions()
{
    _currentInstructions.Erase();
    _allInstructions.Erase();
    _position+=PhysicVector{-_position.GetX(),-_position.GetY()};
    _velocity+=PhysicVector{-_velocity.GetX(),-_velocity.GetY()};
    startTime = 0;
}

void Car::PrintToTable(QTableWidget* table,int param)
{
    QStringList headers;
    headers << "Direction" << "Velocity" << "Time" << "Turn Radius";
    table->clear();
    table->setRowCount(0);
    table->setHorizontalHeaderLabels(headers);
    Queue<std::shared_ptr<Instruction>> queueCopy;
    if(param==0)
        queueCopy = _allInstructions;
    else
        queueCopy = _currentInstructions;
    while(queueCopy.Count()>0)
    {
        auto iterator = queueCopy.Head();
        table->setRowCount(table->rowCount() + 1);
        QTableWidgetItem* item = new QTableWidgetItem;
        if(iterator->GetType() == "Velocity")
            item->setText("Straight");
        else if(iterator->GetDirection() == Direction::LEFT)
            item->setText("Left");
        else
            item->setText("Right");
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(table->rowCount() - 1, 0, item);
        QTableWidgetItem* item2 = new QTableWidgetItem;
        QString velocityString = QString::number(iterator->GetVelocity().GetX());
        velocityString+=" ; ";
        velocityString+=QString::number(iterator->GetVelocity().GetY());
        item2->setText('('+velocityString+')');
        item2->setTextAlignment(Qt::AlignCenter);
        table->setItem(table->rowCount() - 1, 1, item2);
        QTableWidgetItem* item3 = new QTableWidgetItem;
        item3->setText(QString::number(iterator->GetTime()));
        item3->setTextAlignment(Qt::AlignCenter);
        table->setItem(table->rowCount() - 1, 2, item3);
        QTableWidgetItem* item4 = new QTableWidgetItem;
        if(iterator->GetType() == "Velocity")
            item4->setText("-");
        else
            item4->setText(QString::number(iterator->GetTurnRadius()));
        item4->setTextAlignment(Qt::AlignCenter);
        table->setItem(table->rowCount() - 1, 3, item4);
        queueCopy.Dequeue();
    }
}
