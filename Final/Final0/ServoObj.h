#ifndef ServoObj_H
#define ServoObj_H

#include <Servo.h>
#include "Motor.h"

class ServoObj : public Motor
{
  private:
  int pin;
  Servo servo;

  const int mainSpeed = 100;//degree per second
  const int  motorAcceleration = 40; //degree per second squared
  float currentPos = 0;//degree
  float targetPos = 0;
  float maxPos = 180;
  float minPos = 0;
  float posThreshold = 0.1;
  bool arrived = true;
  
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
    setAccel(motorAcceleration);
    setTargetVel(mainSpeed);
  }
  void BaseUpdate(int dt) override
  {
    Motor::BaseUpdate(dt);
    Print(currentPos);
    if (abs(currentPos - targetPos) < posThreshold)
    {
      arrived = true;
      currentPos = targetPos;
      return;
    }
    float expectedDestinationTime = abs((targetPos-currentPos)/currentVel);
    float deAccelTime = abs(currentVel)/accel;
    if (deAccelTime>=expectedDestinationTime)
    {
      currentVel -= 2*dt*accel/1000*sign(currentVel);
    }
    float dp = currentVel*dt/1000;
    currentPos += dp;
    applyPos(currentPos);
  }
  void setTargetPos(float pos)
  {
    targetPos = pos;
    if (targetPos!=currentPos)
    {
      setTargetVel(abs(targetVel)*sign(targetPos - currentPos));
    }
    arrived = false;
     Print("Current position is now this value after changing target, " + String(currentPos),true);
  }
  void applyPos(float pos)
  {
    if (pos>180) {pos = 180;}
    if (pos<0) {pos = 0;}
    currentPos = pos;
    servo.write(pos);
  }
  float getPos()
  {
    return currentPos;
  }
  bool hasArrived()
  {
    return arrived;
  }
};
#endif
