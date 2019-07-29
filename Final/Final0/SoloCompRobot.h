#ifndef SoloCompRobot_H
#define SoloCompRobot_H

#include "Encoder.h"
#include "WallFollower.h"
#include "FullDistanceController.h"

class SoloCompRobot : public GameObject
{
  protected :
    FullDistanceController* distControl;
    WallFollower* wallFollower;
     Encoder* rightEncoder;
    Encoder* leftEncoder;
    const float backUpDistance = 0.05;
    const float forwardDistance = 2;
    const int maxBackUpTime = 1000;
    unsigned long int backUpStartTime = 0;
    
  enum State
  {
    followingWall,
    backingUp,
    goingForward,
    waiting,
  };
  State state = waiting;
  
  public:
  
  SoloCompRobot(FullDistanceController* dc, WallFollower* wf, Encoder* rightEnc, Encoder* leftEnc)
  {
    distControl = dc;
    wallFollower = wf;
    rightEncoder = rightEnc;
    leftEncoder = leftEnc;
  }
  void Update(int dt) override
  {
    switch (state)
    {
      case goingForward:
        if ((*distControl).hasArrived())
          followWall();
        break;
      case followingWall:
        if ((*leftEncoder).isStopped() && (*rightEncoder).isStopped())
        {
            Print("Time to back up",true);
            backUp();
        }
         break;
     case backingUp:
        if (((millis()-backUpStartTime)>maxBackUpTime)||(*distControl).hasArrived())
        {
            Print("back to wall following",true);
            collectCheese();
        } 
    }
  }
  void goForward()
  {
    (*distControl).go(forwardDistance);
    (*wallFollower).enabled = false;
    state = goingForward;
  }
  void followWall()
  {
    (*wallFollower).enabled = true;
    (*distControl).setEnabled(false);
    state = followingWall;
  }
  void collectCheese()
  {
    followWall();
  }
  void backUp()
  {
    backUpStartTime = millis();
    (*distControl).go(-backUpDistance);
    (*wallFollower).enabled = false;
    state = backingUp;
  }
};
#endif
