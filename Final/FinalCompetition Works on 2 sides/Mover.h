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
  
  virtual void halt()
  {
    setVel(0);
    setDelta(0);
  }
  virtual void setVel(int vel)
  {
    (*rightMotor).setTargetVel(vel);
    (*leftMotor).setTargetVel(vel);
  }
  virtual void setDelta(int delta)
  {
    (*rightMotor).setDelta(delta);
    (*leftMotor).setDelta(delta);
  }
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
