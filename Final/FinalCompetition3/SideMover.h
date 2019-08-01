#ifndef SideMover_H
#define SideMover_H

#include "Mover.h"
#include "DCMotor.h"
#include "LineSensor.h"

#define estimatedWallFollowDistanceToCheese  1.45//meters
#define estimatedReturnDistance  1.15//meters
#define distanceAfterLine 0.18
#define unstuckDistance 0.05
#define cheeseTurnAmount 160.0
#define finalTurnAmount 56.0
#define distanceToWallAfterTurn 0.6
#define leftLineSenseThreshold 600
#define rightLineSenseThreshold 500

#define rightTargetDistance 33.0
#define leftTargetDistance 29.0
#define backUpDistance 0.55
#define maxBackUpTime 4000
#define  backUpFromWallDistance 0.17
#define  driveToWallTime 2000
#define  maxUnstuckTime 1000//ms

    
class SideMover : public Mover
{
  protected :
  
    
    unsigned long int timer = 0;
   
    
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
      drivingToWall,
      backingUpFromCheese,
      backingUpFromWall,
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
        if ((*distControl).hasArrived() || (millis()- timer)>maxUnstuckTime)
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
         turn(finalTurnAmount,side);
         state = turningToWall;
       }
       break;
     case backingUpFromCheese:
     if((*distControl).hasArrived() || (millis()- timer)>maxBackUpTime)
     {
      turn(cheeseTurnAmount,left);
      state = turningBack;
     }
      break;
     case turningToWall:
        if((*distControl).hasArrived())
         {
            go(distanceToWallAfterTurn);
           timer = millis();
            state = drivingToWall;
         }
         break;
     case drivingToWall:
        if((*distControl).hasArrived() || (millis()-timer)>driveToWallTime)
         {
            go(-backUpFromWallDistance);
            state = backingUpFromWall;
         }
         break;
     case backingUpFromWall:
     if((*distControl).hasArrived())
     {
            halt();
            state = waiting;
     }
      break;
    }
  }
  void setSide(int newSide)
  {
    side = newSide;
    short int sense;
    float dist;
    if (side == left)
    {
      sense = leftLineSenseThreshold;
      dist = leftTargetDistance;
    } else
    {
      sense = rightLineSenseThreshold;
      dist = rightTargetDistance;
    }
    (*rightLineSensor).setSensitivity(sense);
    (*leftLineSensor).setSensitivity(sense);
    (*wallFollower).setDistance(dist);
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
     (*distControl).applyControl(true);
     
    (*wallFollower).enabled = false;
    
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
    state = backingUpFromCheese;
    timer = millis();
    go(-backUpDistance);
    
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
    (*rightMotor).setDelta(delta/2);
    (*leftMotor).setDelta(-delta/2);
  }
  bool isWaiting() override
  {
    return (state==waiting);
  }
};
#endif
