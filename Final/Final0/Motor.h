#ifndef Motor_H
#define Motor_H

#include "MyMath.h"
#include "GameObject.h"

class Motor : public GameObject
{
  protected :
  float accel = 0;//degree/s^2
  float currentVel = 0;//degree/s
  float targetVel = 0;
  
  public:
  
  void BaseUpdate(int dt) override
  {
    float dv = 0;
    if (accel == 0)
    { 
      currentVel = targetVel;
    } else
    {
      dv = accel*dt/1000.0*sign(targetVel-currentVel);
    }
    if (abs(currentVel-targetVel + dv) < abs(dv))
    {
      currentVel = targetVel;
    }else
    {
      currentVel += dv;
    }
    applyVel();
  }
  void setAccel(float accelVal)
  {
    accel = accelVal;
  }
  void setVel(float vel)
  {
    currentVel = vel;
    setTargetVel(vel);
  }
  void setTargetVel(float vel)
  {
    targetVel = vel;
  }
  virtual void applyVel()
  {
  }
    int getVel()
  {
    //getter for the motor's nominal velocity
    return currentVel;
  }
  

};
#endif
