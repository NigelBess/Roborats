#ifndef Mover_H
#define Mover_H
#include "DCMotor.h"

#include "FullDistanceController.h"

#define right 1
#define left -1

class Mover : public GameObject
{ 
  protected:
      DCMotor* rightMotor;
      DCMotor* leftMotor;
  public:
  
 
  virtual bool isWaiting()
  {
    return false;
  }
  virtual void goToCheese()
  {
  }
  virtual void returnFromCheese()
  {
  }
  virtual void go(float distance)
  {
  }
  virtual void turnToWall()
  {
    }
};
#endif
