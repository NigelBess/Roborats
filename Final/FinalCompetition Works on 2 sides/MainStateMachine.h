#ifndef MainStateMachine_H
#define MainStateMachine_H

#include "Mover.h"
#include "Grabber.h"

class MainStateMachine : public GameObject
{
  protected :
    Mover* mover;
    Grabber* grabber;
  enum State
  {
    goingToCheese,
    grabbingCheese,
    returningFromCheese,
    placingCheeseOnWall,
    resetting,
    waiting,
  };
  State state = waiting;
  
  public:
  
  MainStateMachine(Grabber* g)
  {
    grabber = g;
  }
  void setMover(Mover* m)
  {
    mover = m;
  }
  void Update(int dt) override
  {
    switch (state)
    {
      case goingToCheese:
        if ((*mover).isWaiting())
        {
            Serial.println("robot is trying to grab the cheese...");
            (*grabber).grabCheese();
            state = grabbingCheese;
        }
         break;
     case grabbingCheese:
        if ((*grabber).isWaiting())
        {
            (*mover).returnFromCheese();
            state = returningFromCheese;
        }
        break;
     case returningFromCheese:
      if((*mover).isWaiting())
      {
        (*grabber).lower();
        state = placingCheeseOnWall;
      }
      
      break;
    }
  }
  void collectCheese()
  {
    (*grabber).wait();
    (*mover).goToCheese();
    state = goingToCheese;
  }
  void go(float distance)
  {
    (*mover).go(distance);
  }
};
#endif
