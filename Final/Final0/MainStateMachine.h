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
            Print("robot is trying to grab the cheese...",true);
            grabCheese();
            state = grabbingCheese;
        }
         break;
     case grabbingCheese:
        if ((*grabber).isWaiting())
        {
            (*mover).returnFromCheese();
            state = returningFromCheese;
        } 
    }
  }
  void collectCheese()
  {
    (*grabber).wait();
    (*mover).goToCheese();
    state = goingToCheese;
  }
  void grabCheese()
  {
    (*grabber).grabCheese();
  }
  void returnFromCheese()
  {
      (*mover).returnFromCheese();
  }
};
#endif
