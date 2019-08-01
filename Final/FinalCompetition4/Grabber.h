#ifndef Grabber_H
#define Grabber_H

#include "ServoObj.h"

#define armDownPos 0
#define armUpPos 59
#define armForwardPos 145
#define handClosedPos 0
#define handOpenPos 180

class Grabber : public GameObject
{
  protected :

   ServoObj* armServo;
   ServoObj* handServo;

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
    (*handServo).setTargetVel(100);
    (*handServo).setAccel(400);
  }
  void Update(int dt) override
  {
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
    lift();
  }
  void liftNoPinch()
  {
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
    state = pinching;
   (*handServo).setTargetPos(handClosedPos);
  }
  void openHand()
  {
    (*handServo).setTargetPos(handOpenPos);
  }
  void reset()
  {
    state = waiting;
    (*armServo).setTargetPos(armDownPos);
    (*handServo).setTargetPos(handClosedPos);
  }
  void lower()
  {
    state = lowering;
    (*armServo).setTargetPos(armForwardPos);
  }
  void wait()
  {
    state = waiting;
  }
  bool isWaiting()
  {
    return state == waiting;
  }
  
};
#endif
