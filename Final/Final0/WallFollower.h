#ifndef WallFollower_H
#define WallFollower_H
#include "DCMotor.h"
#include "RangeSensor.h"

class WallFollower : public GameObject
{
  protected :
    DCMotor* rightMotor;
    DCMotor* leftMotor;
    RangeSensor* sensor;
    int direction = 1;
    float k = 30.0;
    int maxDelta;
    float targetDistance = 40;
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
  void setGain(float gain)
  {
    k = gain;
  }
  void setTargetDistance(float distance)
  {
    targetDistance = distance;
  }
  void Update(int dt) override
  {
    if (sensor == NULL) {return;}
    float distance = (*sensor).getDistance();
    float error = (targetDistance - distance);
    float delta = error*k*direction;

        //saturate delta value
    if (delta>maxDelta) {delta = maxDelta;}
    if (delta<-maxDelta) {delta = -maxDelta;}

    setDelta(delta);
  }
  void setMaxDelta(int maxDeltaVal)
  {
    maxDelta = maxDeltaVal;
  }
  void setDelta(float delta)
  {
    //distribute the speed delta evenly accross the two motors
    (*rightMotor).setDelta(delta/2);
    (*leftMotor).setDelta(-delta/2);
  }
  
  
};
#endif
