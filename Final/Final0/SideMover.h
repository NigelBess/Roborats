#ifndef SideMover_H
#define SideMover_H

#include "Mover.h"
#include "DCMotor.h"
#include "LineSensor.h"
class SideMover : public Mover
{
  protected :
    const float estimatedWallFollowDistanceToCheese = 1.5;//meters
    const float estimatedReturnDistance = 1.5;//meters

    int side = left;
    RangeSensor* rightRangeSensor;
    RangeSensor* leftRangeSensor;
    FullDistanceController* distControl;
    WallFollower* wallFollower;
    LineSensor* lineSensor;
    
    enum State
    {
      firstWallFollow,
      turningBack,
      returnFollow,
      turningToWall,
      waiting,
    };
    State state = waiting;
      
  public:
  
  SideMover(DCMotor* rightM, DCMotor* leftM, RangeSensor* rrs, RangeSensor* lrs, FullDistanceController* dc, WallFollower* wf,LineSensor* ls)
  {
    rightMotor = rightM;
    leftMotor = leftM;
    distControl = dc;
    wallFollower = wf;
    rightRangeSensor = rrs;
    leftRangeSensor = lrs;
    lineSensor = ls;
  }
  void Update(int dt) override
  {
    switch (state)
    {
      case firstWallFollow:
        if ((*distControl).hasArrived() && (*lineSensor).isOverLine())
        {
          halt();
          Print("reached the golden cheese!",true);
          state = waiting;
        }
         break;
     case turningBack:
        if ((*distControl).hasArrived())
        {
          Print("Just finished turning arround",true);
          side = -side;
          followWallForDistance(side,estimatedReturnDistance);
          state = returnFollow;
        }
        break;
     case returnFollow:
       if((*distControl).hasArrived())
       {
          Print("Probably back home now",true);
          halt();
          (*distControl).turn(90,side);
          state = turningToWall;
       }
       break;
     case turningToWall:
        if((*distControl).hasArrived())
         {
            halt();
            state = waiting;
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
  void goToCheese() override
  {
    state = firstWallFollow;
    followWallForDistance(side,estimatedWallFollowDistanceToCheese);
  }
   void returnFromCheese() override
  {
    Print("Time to head home from cheese",true);
    (*distControl).setEnabled(true);
    (*distControl).turnAround(-side);
    (*wallFollower).enabled = false;
    (*distControl).applyControl(true);
    state = turningBack;
  }
  void halt()
  {
    (*wallFollower).enabled = false;
    (*distControl).setEnabled(false);
    setVel(0);
    setDelta(0);
    
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
  bool isWaiting() override
  {
    return (state==waiting);
  }
 
};
#endif
