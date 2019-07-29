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
    const float distanceAfterLine = 0;
    const float unstuckDistance = 0.05;

    unsigned long int unstuckStartTime = 0;
    const int maxUnstuckTime = 1000;//ms
    
    int side = right;
    RangeSensor* rightRangeSensor;
    RangeSensor* leftRangeSensor;
    FullDistanceController* distControl;
    WallFollower* wallFollower;
    LineSensor* rightLineSensor;
   LineSensor* leftLineSensor;
    
    enum State
    {
      firstWallFollow,
      turningBack,
      returnFollow,
      turningToWall,
      waiting,
      goingSetDistance,
      gettingUnstuck,
    };
    State state = waiting;
    State previousState = waiting;
      
  public:
  
  SideMover(DCMotor* rightM, DCMotor* leftM, RangeSensor* rrs, RangeSensor* lrs, FullDistanceController* dc, WallFollower* wf,LineSensor* rls,LineSensor* lls)
  {
    rightMotor = rightM;
    leftMotor = leftM;
    distControl = dc;
    wallFollower = wf;
    rightRangeSensor = rrs;
    leftRangeSensor = lrs;
    rightLineSensor = rls;
    leftLineSensor = lls;
  }
  void Update(int dt) override
  {
//    if(state!=waiting && (*distControl).isStuck())
//    {
//      previousState = state;
//      unstuckStartTime = millis();
//      (*distControl).go(-unstuckDistance);
//      state = gettingUnstuck;
//    }
    switch (state)
    {
      case gettingUnstuck:
        if ((*distControl).hasArrived() || (millis()-unstuckStartTime)>maxUnstuckTime)
        {
          state = previousState;
        }
      break;
      case firstWallFollow:
        if ((*distControl).hasArrived() && ((*leftLineSensor).isOverLine() || (*rightLineSensor).isOverLine()))
        {
          halt();
          Print("found the line!");
          go(distanceAfterLine);
          state = goingSetDistance;
        }
         break;
     case goingSetDistance:
      if((*distControl).hasArrived())
      {
          halt();
          Serial.println("reached the golden cheese!");
          state = waiting;
      }
      break;
     case turningBack:
        if ((*distControl).hasArrived())
        {
          Serial.println("Just finished turning arround");
          side = -side;
          followWallForDistance(side,estimatedReturnDistance);
          state = returnFollow;
        }
        break;
     case returnFollow:
       if((*distControl).hasArrived())
       {
         Serial.println("Probably back home now");
         turn(90,side);
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
  void go(float distance) override
  {
     setDelta(0);
     (*distControl).setEnabled(true);
    (*wallFollower).enabled = false;
    (*distControl).applyControl(true);
    (*distControl).go(distance);
  }
  void turn(float degrees,int direction)
  {
    halt();
    (*distControl).setEnabled(true);
    (*distControl).applyControl(true);
    (*distControl).turn(degrees,direction);
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
    Serial.println("Time to head home from cheese");
    turn(160,side);
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
    (*rightMotor).setVel(vel);
    (*leftMotor).setVel(vel);
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
