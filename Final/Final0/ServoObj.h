#ifndef ServoObj_H
#define ServoObj_H

#include <Servo.h>
#include "Motor.h"

class ServoObj : public Motor
{
  private:
  int pin;
  Servo servo;

  float currentPos = 0;//degree
  float targetPos = 0;
  float nextTargetPos = -1;
  float maxPos = 180;
  float minPos = 0;
  uint8_t posThreshold = 1;
  
  public:
  ServoObj(int pinNum)
  {
    pin = pinNum;
     servo.write(0);
  }
  void Awake() override
  {
    servo.attach(pin);
    Print("servo awake");
  }
  void BaseUpdate(int dt) override
  {
    Motor::BaseUpdate(dt);
    if (abs(currentPos - targetPos) < posThreshold)
    {
      currentPos = targetPos;
      setTargetPos(nextTargetPos);
      return;
    }
    float expectedDestinationTime = (targetPos-currentPos)/currentVel;
    float deAccelTime = abs(currentVel)/accel;
    if (deAccelTime>=expectedDestinationTime)
    {
      currentVel -= 2*dt*accel/1000*sign(currentVel);
    }
    float dp = currentVel*dt/1000;
    currentPos += dp;
    applyPos(currentPos);
  }
  void setPos(float pos)
  {
    pos = MyMath::clamp(pos,minPos,maxPos);
    currentPos = pos;
    setTargetPos(pos);
  }
  void setTargetPos(float pos)
  {
    targetPos = pos;
    if (targetPos!=currentPos)
    {
      setTargetVel(abs(targetVel)*sign(targetPos - currentPos));
    }
    if (nextTargetPos == -1)
    {
      nextTargetPos = targetPos;
    }
  }
  void setNextTargetPos(float pos)
  {
    nextTargetPos = pos;
  }
  void applyPos(float pos)
  {
    pos = MyMath::clamp(pos,0,180);
    currentPos = pos;
    servo.write(pos);
  }
  float getPos()
  {
    return currentPos;
  }
};
#endif
