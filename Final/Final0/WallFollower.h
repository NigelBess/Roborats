#ifndef WallFollower_H
#define WallFollower_H
#include "DCMotor.h"
#include "RangeSensor.h"
#include "MyMath.h"

class WallFollower : public GameObject
{
  protected :
    DCMotor* rightMotor;
    DCMotor* leftMotor;
    RangeSensor* sensor;
    int direction = 1;
    float k = 30.0;
    int maxDelta = 510;
    float targetDistance = 40;
    bool fullControl = true;
    int maxSpeed = 255;
    bool possiblyAtCorner = false;
    unsigned long int cornerDetectionStartTime = 0;
    const int cornerDetectionTimeThreshold = 1000;//ms
    bool atCorner = false;
  public:
  WallFollower(DCMotor* right, DCMotor* left, float gain)
  {
    rightMotor = right;
    leftMotor = left;
    setGain(gain);
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
    delta = MyMath::saturate(delta,float(maxDelta));
    Print(delta);
    if (!atCorner && delta >=maxDelta)
    {
      if(!possiblyAtCorner)
      {
        possiblyAtCorner = true;
        cornerDetectionStartTime = millis();
      } else
      {
        if ((millis()-cornerDetectionStartTime)>cornerDetectionTimeThreshold)
        {
          atCorner = true;
        }
      }
    } else
    {
      atCorner = false;
      possiblyAtCorner = false;
    }
    setDelta(delta);
  }
  void setMaxDelta(int maxDeltaVal)
  {
    maxDelta = maxDeltaVal;
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
  bool isAtCorner()
  {
    return atCorner;
  }
  
  
};
#endif
