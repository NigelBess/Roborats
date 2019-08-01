#ifndef ServoObj_H
#define ServoObj_H

#include <Servo.h>
#include "Motor.h"

#define posThreshold 2.0
#define servoMainSpeed 50//degree per second
#define  servoMotorAcceleration 30 //degree per second squared

class ServoObj : public Motor
{
  private:
  int pin;
  Servo servo;

  
  float currentPos = 0;//degree
  float targetPos = 0;
  
  bool arrived = true;

  
  public:
  ServoObj(int pinNum)
  {
    pin = pinNum;
     servo.write(0);
      setAccel(servoMotorAcceleration);
    setTargetVel(servoMainSpeed);
  }
  void Awake() override
  {
    servo.attach(pin);
   
  }
  void BaseUpdate(int dt) override
  {
    applyPos();
    if (arrived) {currentVel = 0; return;}
    Motor::BaseUpdate(dt);
    if (abs(currentPos - targetPos) < posThreshold)
    {
      arrived = true;
      currentVel = 0;
      currentPos = targetPos;
      return;
    }
    float expectedDestinationTime = abs((targetPos-currentPos)/currentVel);
    float deAccelTime = abs(currentVel/accel);
    if (deAccelTime>=expectedDestinationTime)
    {
      currentVel -= 2*dt*accel/1000*sign(currentVel);
    }
    float dp = currentVel*dt/1000;
    if(abs(dp)>3.0)
    {
      dp = 0;
      currentVel = 0;
    }
    currentPos += dp;
    
    
  }
  void setTargetPos(float pos)
  {
    targetPos = pos;
    if (targetPos!=currentPos)
    {
      setTargetVel(abs(targetVel)*sign(targetPos - currentPos));
      arrived = false;
    }
  }
  
  void applyPos()
  {
    if (currentPos>180) {currentPos = 180;}
    if (currentPos<0) {Serial.println(currentPos);currentPos = 0;}
    servo.write(currentPos);
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
