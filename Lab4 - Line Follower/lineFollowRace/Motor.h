//make sure this class has not been included twice
#ifndef Motor_H
#define Motor_H

#include <AFMotor.h>

class Motor
//this class allows the motor to be controlled by setting a nominal velocity and an optional 
// offset from nominal velocity used for turning.
// the class also keeps track of these values, and encapsulates data about the motor
{
  private:
  AF_DCMotor *motorObj = 0;//points to the motor that this controls
  int nominalVel = 0;//nominal velocity of motor
  float delta = 0;//delta is the offset from the nominal velocity (used for turning)
  bool reverse = false;//is the motor polarity reversed?
  
  void applyVel()
  {
    //using nominal velocity and delta, set motor speed and direction

    //total velocity = nominal + delta
    int vel = nominalVel + int(delta);
    
    (*motorObj).setSpeed(min(abs(vel),255));// set speed appropriately and saturate to 255

    //if polarity is reversed, flip the velocity direction
    if (reverse)
    {
      vel = -vel;
    }

    //move motor in the direction specified by velocity
    if (vel>0)
    {
      (*motorObj).run(FORWARD);
    } else
    {
      (*motorObj).run(BACKWARD);
    }
  }

  
  public:
  Motor(int pin)
  {
   //setup the AFMotor with the appropriate pin
   setPin(pin);
  }
  
  Motor(int pin, bool runReverse)
  {
    //same as the other constructor, but allows definition of polarity
    setPin(pin);
    reverse = runReverse;
  }
  
  void setPin(int pin)
  {
    //use appropriate AFMotor constructors, dependent on which pin was chosen
     if (pin<3)
    {
      motorObj = new AF_DCMotor(pin,MOTOR12_1KHZ);
    } else
    {
      motorObj = new AF_DCMotor(pin,MOTOR34_1KHZ);
    }
  }
  
  void setVel(int vel)
  {
    //sets nominal velocity and updates AFMotor
    nominalVel = vel;
    applyVel();
  }
  
  void setDelta(float value)
  {
    //sets delta in velocity and updates AFMotor
    delta = value;
    applyVel();
  }
  
  int getVel()
  {
    //getter for the motor's nominal velocity
    return nominalVel;
  }

};
#endif
