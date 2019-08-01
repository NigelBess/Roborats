#ifndef Grabber_H
#define Grabber_H

#include "ServoObj.h"

class Grabber : public GameObject
{
  protected :
  const uint8_t armDownPos = 0;
  const uint8_t armUpPos = 72;
  const uint8_t armForwardPos = 150;
  const uint8_t handClosedPos = 0;
  const uint8_t handOpenPos = 90;
   ServoObj* armServo;
   ServoObj* handServo;

  enum State
  {
    lifting,
    pinching,
    lowering,
    opening,
    waiting,  
  };
  State state = waiting;
  
  public:
  
  Grabber(ServoObj* aS, ServoObj* hS)
  {
    armServo = aS;
   
    handServo = hS;
    (*handServo).setTargetVel(200);
    (*handServo).setAccel(80);
  }
  void Update(int dt) override
  {
    switch (state)
    {
      case lifting:
        if ((*armServo).hasArrived())
        {
          Serial.println("grabber has probably reached the cheese");
          pinch();
        }
         break;
     case pinching:
        if ((*handServo).hasArrived())
        {
          Serial.println("finsihed pinching!");
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
