#ifndef SideMover_H
#define SideMover_H

#include "Mover.h"
#include "DCMotor.h"
class SideMover : public Mover
{
  protected :
    const float distanceAfterCorner = 1;//m

    const int mainSpeed = 255;
    const int  motorAcceleration = 255; // counts per second
    const float  wallFollowerDistance = 30.0;
    const float estimatedWallFollowDistanceToCheese = 1.8;
    
    RangeSensor* rightRangeSensor;
    RangeSensor* leftRangeSensor;
    FullDistanceController* distControl;
    WallFollower* wallFollower;
    bool returning = false;
    
    enum State
    {
      firstWallFollow,
      goingForward,
      waiting,
    };
    State state = waiting;
      
  public:
  
  SideMover(DCMotor* rightM, DCMotor* leftM, RangeSensor* rrs, RangeSensor* lrs, FullDistanceController* dc, WallFollower* wf)
  {
    rightMotor = rightM;
    leftMotor = leftM;
    distControl = dc;
    wallFollower = wf;
    rightRangeSensor = rrs;
    leftRangeSensor = lrs;
  }
  void Awake() override
  {
    (*wallFollower).setTargetDistance(wallFollowerDistance);
    Motor* motors[] = {rightMotor,leftMotor};
    for (int i = 0; i < 2; i++)
    {
      (*motors[i]).setAccel(motorAcceleration);
      (*motors[i]).setTargetVel(mainSpeed);
    }
  }
  void Update(int dt) override
  {
    switch (state)
    {
      case firstWallFollow:
        if ((*distControl).hasArrived())
        {
          Print("arrived!",true);
          halt();
        }
       break;
      case goingForward:
        if((*distControl).hasArrived())
        {
          
          halt();
        } 
    }
  }
  void FollowWall(int direction)
  { 
    
    (*wallFollower).enabled = true;
    (*wallFollower).allowFullVelocityControl(true);
    (*distControl).setEnabled(true);
    (*distControl).applyControl(false);
    RangeSensor* sense = rightRangeSensor;
    if (direction<0)  
    {
      sense = leftRangeSensor;
    }
    (*wallFollower).setSensor(sense);
    (*wallFollower).setDirection(direction);
  }
  void go(float distance)
  {
     setDelta(0);
     (*distControl).setEnabled(true);
    (*wallFollower).enabled = false;
    (*distControl).applyControl(true);
    (*distControl).go(distance);
  }
  void followWallForDistance(int direction,float distance)
  {
    (*distControl).go(distance);
    FollowWall(direction);
  }
  void goToCheese()
  {
    returning = false;
    state = firstWallFollow;
    followWallForDistance(right,estimatedWallFollowDistanceToCheese);
  }
  void halt()
  {
    (*wallFollower).enabled = false;
    (*distControl).setEnabled(false);
    setVel(0);
    setDelta(0);
    state = waiting;
  }
  void setVel(int vel)
  {
    (*rightMotor).setTargetVel(vel);
    (*leftMotor).setTargetVel(vel);
  }
  void setDelta(int delta)
  {
    (*rightMotor).setDelta(delta);
    (*leftMotor).setDelta(delta);
  }
};
#endif
