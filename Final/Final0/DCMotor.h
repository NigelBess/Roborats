#ifndef DCMotor_H
#define DCMotor_H
#include "Motor.h"
#include <AFMotor.h>

class DCMotor : public Motor
{
  protected :
  AF_DCMotor *motorObj = 0;//points to the motor that this controls
  bool reverse = false;//is the motor polarity reversed?
  int delta;
  Encoder* encoder;
  const int threshold = 120;
  const int effectiveZeroSpeed = 1;
  const int mainSpeed = 255;
  const int  motorAcceleration = 510; // counts per second
  
  public:
  DCMotor(int pin)
  {
   //setup the AFMotor with the appropriate pin
   setPin(pin);
  }
  DCMotor(int pin, bool runReverse)
  {
    setPin(pin);
    reverse = runReverse;
  }
  void setEncoder(Encoder* enc)
  {
    encoder = enc;
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
    int mappedSpeed = map(cappedSpeed,0,255,threshold,255);
    if(abs(vel)<=effectiveZeroSpeed)
    {
      mappedSpeed = 0;
      vel = 0;
    }
    
    (*motorObj).setSpeed(mappedSpeed);
    if(encoder!=NULL)
    {
      (*encoder).setDirection(sign(vel));
    }
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
