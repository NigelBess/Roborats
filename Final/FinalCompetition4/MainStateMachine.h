#ifndef MainStateMachine_H
#define MainStateMachine_H

//this object deals with the cheese collection algorithm at its highest level of abstraction

#include "SideMover.h"
#include "Grabber.h"

class MainStateMachine : public GameObject
{
  protected :
    //references to object controlling wheel motion and object controlling arm motion
    SideMover* mover;
    Grabber* grabber;

    //should the robot get both of the side hanging cheeses?
    bool getBoth = false;

    //state machine states
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
    //passes reference to side mover object
    mover = m;
  }

  
  void Update(int dt) override
  {

    //state machine
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
    //begins cheese collection algorithm
    (*grabber).wait();
    (*mover).goToCheese();
    state = goingToCheese;
  }
  void getBothCheeses()
  {
   getBoth = true; 
  }
};
#endif
