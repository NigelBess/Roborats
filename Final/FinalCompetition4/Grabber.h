#ifndef Grabber_H
#define Grabber_H

//object that controls motion of the grabbing arm

#include "ServoObj.h"

//servo positions
#define armDownPos 0
#define armUpPos 59
#define armForwardPos 145
#define handClosedPos 0
#define handOpenPos 180

class Grabber : public GameObject
{
  protected :
  //references to servo objects
   ServoObj* armServo;
   ServoObj* handServo;

  //states in the state machine
  enum State
  {
    lifting,
    liftingNoPinch,
    pinching,
    lowering,
    reLifting,
    opening,
    waiting,  
  };
  State state = waiting;
  
  public:
  
  Grabber(ServoObj* aS, ServoObj* hS)
  {
    armServo = aS;
    handServo = hS;
  }

  void Awake() override
  {
    //set hand servo speed/acceleration
    (*handServo).setTargetVel(100);
    (*handServo).setAccel(400);
  }
  void Update(int dt) override
  {
    //state machine
    switch (state)
    {
      case liftingNoPinch:
      if ((*armServo).hasArrived())
        {
          state = waiting;
        }
         break;
      case lifting:
        if ((*armServo).hasArrived())
        {
          pinch();
        }
         break;
     case pinching:
        if ((*handServo).hasArrived())
        {

          reset();
        }
        break;
      case lowering:
       if ((*armServo).hasArrived())
        {
          (*handServo).setTargetPos(handOpenPos);
          state = opening;
        }
      break;
      
      case opening:
        if ((*handServo).hasArrived())
        {
          (*armServo).setTargetPos(armUpPos);
          state = reLifting;
        }
      break;
      case reLifting:
       if ((*armServo).hasArrived())
        {
          reset();
          state = waiting;
        }
      break;
        
         
    }
  }
  void grabCheese()
  {
    //starts the sequence of motions that make the grabber grab the cheese and lower it
    lift();
  }
  void liftNoPinch()
  {
    //lifts the arm, leaving the hand open
    state = liftingNoPinch;
    (*armServo).setTargetPos(armUpPos);
    (*handServo).setTargetPos(handOpenPos);
  }
  void lift()
  {
    state = lifting;
    (*armServo).setTargetPos(armUpPos);
    (*handServo).setTargetPos(handOpenPos);
  }
  void pinch()
  {
    //closes hand
    state = pinching;
   (*handServo).setTargetPos(handClosedPos);
  }
  void openHand()
  {
    //opens hand
    (*handServo).setTargetPos(handOpenPos);
  }
  void reset()
  {
    //puts arm back with hand closed
    state = waiting;
    (*armServo).setTargetPos(armDownPos);
    (*handServo).setTargetPos(handClosedPos);
  }
  void lower()
  {
    //lowers arm
    state = lowering;
    (*armServo).setTargetPos(armForwardPos);
  }
  void wait()
  {
    //sets state to waiting
    state = waiting;
  }
  bool isWaiting()
  {
    //is the grabber ready for a new command?
    return state == waiting;
  }
  
};
#endif
