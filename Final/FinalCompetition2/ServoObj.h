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
  float posThreshold = 2;
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
    applyPos();
    if (arrived) {currentVel = 0; return;}
    Motor::BaseUpdate(dt);
    if (abs(currentPos - targetPos) < posThreshold)
    {
      arrived = true;
      Serial.println("arrived!");
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
      Serial.println("high position change detected: "+ String(dp));
      dp = 0;
      currentVel = 0;
    }
    currentPos += dp;
    
    
  }
  void setTargetPos(float pos)
  {
    Serial.println("");
    Serial.println("Current position before changing target, " + String(currentPos));
    Serial.println("setting target position to " + String(pos));
    targetPos = pos;
    if (targetPos!=currentPos)
    {
      setTargetVel(abs(targetVel)*sign(targetPos - currentPos));
      Serial.println("new target velocity: " + String(targetVel));
      arrived = false;
    }
    
     Serial.println("Current position after changing target, " + String(currentPos));
  }
  
  void applyPos()
  {
    if (currentPos>180) {currentPos = 180;}
    if (currentPos<0) {Serial.println(currentPos);currentPos = 0;}
    Print(currentPos);
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
