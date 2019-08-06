#ifndef WallFollower_H
#define WallFollower_H
#include "DCMotor.h"
#include "RangeSensor.h"
#include "MyMath.h"

//proportional controller for following walls

#define kWallFollower 22//gain
#define maxDelta 510//max output
#define cornerDetectionTimeThreshold  1000//ms
    
class WallFollower : public GameObject
{
  protected :
    DCMotor* rightMotor;
    DCMotor* leftMotor;
    RangeSensor* sensor;
    
    int direction;//side that we are following 
    
    float targetDistance = 32;//target distance from wall
    bool fullControl = true;//do we have control of motor nominal velocity too? (or just delta)
    int maxSpeed = 255;//if we have full control, one motor will always have this value for max speed
  public:
  WallFollower(DCMotor* right, DCMotor* left)
  {
    rightMotor = right;
    leftMotor = left;
  }
  void setSensor(RangeSensor* sense)
  {
    //passes reference to range sensor object (for switching between following left or right side)
    sensor = sense;
  }
  
  void setDirection(int dir)
  {
    //public setter
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
    if (sensor == NULL) {return;}//cant follow wall without a sensor

    
    float distance = (*sensor).getDistance();//read distance
    float error = (targetDistance - distance);//get error
    float delta = error*kWallFollower*direction;// p control

        //saturate delta value
    delta = MyMath::saturate(delta,float(maxDelta));
    setDelta(delta);
  }
  
  void setDelta(float delta)
  {
    //applies ouput to motors
    if (fullControl)//if we have full control of motor speeds, set the velocity such that one motor always moves at max speed
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
    //public setter
    fullControl = state;
  }
  void setDistance(float distance)
  {
    //public setter
    targetDistance = distance;
  }
  
  
};
#endif
