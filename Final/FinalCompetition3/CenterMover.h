#ifndef CenterMover_H
#define CenterMover_H
#include "Mover.h"
#include "LineFollower.h"
#include "Grabber.h"
#define firstMoveDistance 0.4
#define firstFollowDistance 0.45
#define secondMoveDistance 0.5
#define secondFollowDistance 0.3
#define upRampDelta 125
#define backUpDistanceFromRamp 0.8
#define turnAroundAmount 180
#define lastDistance 0.95

#define right 1
#define left -1

class CenterMover : public Mover
{ 
  protected:
      DCMotor* rightMotor;
      DCMotor* leftMotor;
      FullDistanceController* distControl;
      LineFollower* lineFollower;
      Grabber* grabber;
      enum State
      {
        firstMove,
        firstFollow,
        secondMove,
        lifting,
        secondFollow,
        backingUp,
        turningAround,
        drivingToWall,
        waiting,
      };
      State state = waiting;
  public:
  CenterMover(DCMotor* rightM, DCMotor* leftM, FullDistanceController* dc,  LineFollower* lf, Grabber* g)
  {
    rightMotor = rightM;
    leftMotor = leftM;
    distControl = dc;
    lineFollower = lf;
    grabber = g;
  }
  void Update(int dt) override
  {
    switch(state)
    {
      case firstMove:
      if((*distControl).hasArrived())
      {
        followForDistance(firstFollowDistance);
        state = firstFollow;
      }
      break;
      case firstFollow:
      if((*distControl).hasArrived())
      {
        go(secondMoveDistance);
        (*distControl).applyControl(false);
        setDelta(upRampDelta);
        setVel(255 - upRampDelta/2);
        state = secondMove;
      }
      break;
      case secondMove:
      if((*distControl).hasArrived())
      {
        (*grabber).liftNoPinch();
        halt();
        state = lifting;
      }
      
      break;
      case lifting:
      if((*grabber).isWaiting())
      {
        followForDistance(secondFollowDistance);
       // (*lineFollower).setSide(right);
        state = secondFollow;
      }
      break;
      
      case secondFollow:
      if((*distControl).hasArrived())
      {
        halt();
        state = waiting;
      }
      break;
      case backingUp:
      if((*distControl).hasArrived())
      {
        turn(turnAroundAmount,1);
        state = turningAround;
      }
      break;
      case turningAround:
       if((*distControl).hasArrived())
      {
        go(lastDistance);
        state = drivingToWall;
      }
      break;
      case drivingToWall:
      if((*distControl).hasArrived())
      {
        halt();
        state = waiting;
      }
      break;
      
    }
  }
  bool isWaiting() override
  {
    return state == waiting;
  }
  void goToCheese() override
  {
    state = firstMove;
    (*grabber).lower();
    (*grabber).openHand();
    go(firstMoveDistance);
  }
  virtual void returnFromCheese()
  {
    go(-backUpDistanceFromRamp);
    state = backingUp;
  }
  void go(float distance)
  {
    (*distControl).setEnabled(true);
     (*distControl).applyControl(true);
    (*distControl).go(distance);
    (*lineFollower).enabled = true;
    (*lineFollower).applyControl(false);
  }
  void followForDistance(float distance)
  {
    (*distControl).setEnabled(true);
     (*distControl).applyControl(false);
    (*distControl).go(distance);
    (*lineFollower).enabled = true;
    (*lineFollower).applyControl(true);
  }
   void halt()
  {
    (*lineFollower).enabled = false;
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
  void turn(float degrees,int direction)
  {
    halt();
    (*distControl).setEnabled(true);
    (*distControl).applyControl(true);
    (*distControl).turn(degrees,direction);
  }
};
#endif
