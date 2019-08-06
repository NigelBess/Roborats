#ifndef DCMotor_H

#define DCMotor_H

//This object allows us to interact with AF_DCMotors in a more intuitive way
//It inherits from Motor, which lets us define acceleration and target velocity to control speed
//It also linearizes the motor's physical speed as a function of velocity defined in software
//And it allows us to define velocity as a nominal velocity plus a delta. This is useful for 
//wall following, in which we want to move both motors at the same average speed and vary 
//their difference in speeds


#include "Motor.h"
#include <AFMotor.h>

#define thresholdSpeed 155 //"speed" (actually voltage) at which the motor starts to turn under load
#define effectiveZeroSpeed 1 //the motor's speed gets set to this value or lower, just set it to zero
#define mainSpeed 255 // default speed
#define  motorAcceleration  510 //speed units per second

class DCMotor : public Motor
{
  protected :
  AF_DCMotor* motorObj = 0;//points to the motor that this controls
  bool reverse = false;//is the motor polarity reversed?
  short int delta;//difference in velocity from nomimal
  //we will define its direction to match the sign of the motors velocity
  
  public:
  DCMotor(int pin)
  {
   //setup the AFMotor with the appropriate pin
   setPin(pin);
  }
  DCMotor(int pin, bool runReverse)
  {
    //overload for defining polarity
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
  void Awake() override
  {
    //set acceleration to default, and speed to zero
    setAccel(motorAcceleration);
    setTargetVel(0);
  }
  void setDelta(float value)
  {
    //sets delta in velocity and updates AFMotor
    delta = value;
    applyVel();
  }
  void applyVel() override
  {
    int vel = currentVel + int(delta);
    int cappedSpeed = min(abs(vel),255);
    int mappedSpeed = map(cappedSpeed,0,255,thresholdSpeed,255);
    if(abs(vel)<=effectiveZeroSpeed)
    {
      mappedSpeed = 0;
      vel = 0;
    }
    
    (*motorObj).setSpeed(mappedSpeed);
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

};
#endif
