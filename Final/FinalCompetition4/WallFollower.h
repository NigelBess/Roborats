#ifndef WallFollower_H
#define WallFollower_H
#include "DCMotor.h"
#include "RangeSensor.h"
#include "MyMath.h"

#define kWallFollower 22
#define maxDelta 510
#define cornerDetectionTimeThreshold  1000//ms
    
class WallFollower : public GameObject
{
  protected :
    DCMotor* rightMotor;
    DCMotor* leftMotor;
    RangeSensor* sensor;
    int direction = 1;
    
    float targetDistance = 32;
    bool fullControl = true;
    int maxSpeed = 255;
  public:
  WallFollower(DCMotor* right, DCMotor* left)
  {
    rightMotor = right;
    leftMotor = left;
  }
  void setSensor(RangeSensor* sense)
  {
    sensor = sense;
  }
  void setDirection(int dir)
  {
    if (dir<0)
    {
      direction = -1;
    } else 
    {
      direction = 1;
    }
  }
  void Update(int dt) override
  {
    if (sensor == NULL) {return;}
    float distance = (*sensor).getDistance();
    float error = (targetDistance - distance);
    float delta = error*kWallFollower*direction;

        //saturate delta value
    delta = MyMath::saturate(delta,float(maxDelta));
    setDelta(delta);
  }
  void setDelta(float delta)
  {
    if (fullControl)
    {
      int vel = maxSpeed-abs(delta/2);
      (*rightMotor).setVel(vel);
      (*leftMotor).setVel(vel);
    }
    //distribute the speed delta evenly accross the two motors
    (*rightMotor).setDelta(delta/2);
    (*leftMotor).setDelta(-delta/2);
  }
  void allowFullVelocityControl(bool state)
  {
    fullControl = state;
  }
  void setDistance(float distance)
  {
    targetDistance = distance;
  }
  
  
};
#endif
