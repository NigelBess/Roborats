#ifndef Motor_H
#define Motor_H

#include "MyMath.h"
#include "GameObject.h"

//This class is intended as a superclass for servos and DCMotors.
//It keeps track of velocity and acceleration information
class Motor : public GameObject
{
  protected :
  float accel = 0;//degree/s^2
  float currentVel = 0;//degree/s
  float targetVel = 0;
  
  public:
  
  void BaseUpdate(int dt) override
  {
    float dv = 0;//change in velocity this loop
    
    if (accel == 0)//use 0 acceleration to mean infinity
    { 
      currentVel = targetVel;
    } else
    {
      dv = accel*dt/1000.0*sign(targetVel-currentVel);//get change in velocity  this frame. The 1000 converts from ms to s
    }
    if (abs(currentVel-targetVel + dv) < abs(dv))//if we are reasonably close to the target velocity set velocity to target. This prevents oscillation
    {
      currentVel = targetVel;
    }else
    {
      currentVel += dv;//increment
    }
    applyVel();//physically apply velocity to motor. On second thought this should have been defined in DCMotor.h because ServoObj.h does not use this method
  }
  void setAccel(float accelVal)
  {
    //setter for acceleration
    accel = accelVal;
  }
  void setVel(float vel)
  {
    //sets velocity without accelerating
    currentVel = vel;
    setTargetVel(vel);
  }
  void setTargetVel(float vel)
  {
    //sets velocity to accelerate to
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
