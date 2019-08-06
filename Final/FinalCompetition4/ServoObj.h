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
    setAccel(servoMotorAcceleration);
    setTargetVel(servoMainSpeed);
  }
  void Awake() override
  {
    //setup servo
    servo.attach(pin);
   
  }
  void BaseUpdate(int dt) override
  {
    applyPos();//sets servo position to match currentPosition
    if (arrived) {currentVel = 0; return;}
    
    Motor::BaseUpdate(dt);//keeps track of velocity and acceleration
    
    if (abs(currentPos - targetPos) < posThreshold)
    {
      arrived = true;
      currentVel = 0;
      currentPos = targetPos;
      return;
    }

    //de accelerate if we are approaching target position
    float expectedDestinationTime = abs((targetPos-currentPos)/currentVel);//how long will it take to reach target position at current velocity?
    float deAccelTime = abs(currentVel/accel);//how long would it take to accelerate to a stop?
    
    if (deAccelTime>=expectedDestinationTime)//if we are close enough to target position, accelerate to a stop
    {
      currentVel -= 2*dt*accel/1000*sign(currentVel);//factor of 2 counteracts the incrementing of currentVelocity in Motor.h
      //in hindsight, this class should not have inherited from Motor
    }
    float dp = currentVel*dt/1000;//change in position this loop

    //theres a bug where position changes too fast. I dont know where it's coming from
    //the temporary solution is to not let the servo change its position by more than 3 degrees in a single loop
    if(abs(dp)>3.0)
    {
      dp = 0;
      currentVel = 0;
    }

    //increment position
    currentPos += dp;
    
    
  }
  void setTargetPos(float pos)
  {
    //define new target position for the servo
    targetPos = pos;
    if (targetPos!=currentPos)
    {
      setTargetVel(abs(targetVel)*sign(targetPos - currentPos)); // appropriately sets direction of velocity so that the servo move in the correct direction
      arrived = false;
    }
  }
  
  void applyPos()
  {
    //clamp position
    if (currentPos>180) {currentPos = 180;}
    if (currentPos<0) {Serial.println(currentPos);currentPos = 0;}

    //update servo output
    servo.write(currentPos);
  }
  
  float getPos()
  {
    //public getter
    return currentPos;
  }
  bool hasArrived()
  {
    //is the servo at its target position?
    //this assumes the physical servo is a zero order system (which is reasonable if we are trying to move it slowly)
    return arrived;
  }
};
#endif
