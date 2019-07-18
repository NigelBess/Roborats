#ifndef ServoObj_H
#define ServoObj_H

#include <Servo.h>
#include "MyMath.h"
#include "GameObject.h"

class ServoObj : public GameObject
{
  private:
  Servo thisServo;
  float currentPos = 0;
  float vel = 100;//degree/s
  public:
  ServoObj(int pin)
  {
    thisServo.attach(pin);
  }
  void Awake() override
  {
    Print("servo awake");
  }
  
  void Update(int dt) override
  {
    Print(currentPos);
    move(vel*float(dt)/float(1000));
    if (currentPos>=180 || currentPos <=0)
    {
      vel = -vel;
    }
  }
  void move(float amount)
  {
    setPos(currentPos + amount);
  }
  void setPos(float pos)
  {
    pos = MyMath::clamp(pos,0,180);
    currentPos = pos;
    Print(pos);
    thisServo.write((int)pos);
  }
  float getPos()
  {
    return currentPos;
  }
};
#endif
