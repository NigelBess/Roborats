#ifndef Grabber_H
#define Grabber_H

#include "ServoObj.h"

class Grabber : public GameObject
{
  protected :
  const int armDownPos = 0;
  const int armUpPos = 120;
  const int handClosedPos = 90;
  const int handOpenPos = 0;
   ServoObj* armServo;
   ServoObj* handServo;

  enum State
  {
    lifting,
    pinching,
    lowering,
    unpinching,
    waiting,  
  };
  State state = waiting;
  
  public:
  
  Grabber(ServoObj* aS, ServoObj* hS)
  {
    armServo = aS;
    handServo = hS;
  }

  void Update(int dt) override
  {
    switch (state)
    {
      case lifting:
        if ((*armServo).hasArrived())
        {
          Print("grabber has probably reached the cheese",true);
          pinch();
        }
         break;
     case pinching:
        if ((*handServo).hasArrived())
        {
          Print("finsihed pinching!",true);
          lower();
        }
        
         
    }
  }
  void grabCheese()
  {
    lift();
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
  void lower()
  {
    state = waiting;
   (*armServo).setTargetPos(armDownPos);
    (*handServo).setTargetPos(handOpenPos);
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
