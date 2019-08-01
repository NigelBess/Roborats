#ifndef MainStateMachine_H
#define MainStateMachine_H

#include "SideMover.h"
#include "Grabber.h"

class MainStateMachine : public GameObject
{
  protected :
    SideMover* mover;
    Grabber* grabber;
    bool getBoth = false;
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
  void setMover(SideMover* m)
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
     case placingCheeseOnWall:
      if((*grabber).isWaiting())
      {
        if (getBoth)
        {
          (*mover).turn(270);
          collectCheese();
        }
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
  void getBothCheeses()
  {
   getBoth = true; 
  }
};
#endif
