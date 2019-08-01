#ifndef LineFollower_H
#define LineFollower_H
#include "LineSensor.h"
#include "DCMotor.h"

#define right 1
#define left -1
#define lineFollowMaxDelta 510
#define kLineFollow 220
#define lineFollowDefaultVel 255

class LineFollower: public GameObject
{
  protected:
   LineSensor* rightLineSensor;
   LineSensor* leftLineSensor;
   DCMotor* rightMotor;
   DCMotor* leftMotor;
   int8_t sideWeAreOn;
   bool hasControl = true;
   
  public:
  LineFollower(LineSensor* rls, LineSensor* lls, DCMotor* rm, DCMotor* lm)
  {
    rightLineSensor = rls;
    leftLineSensor = lls;
    rightMotor = rm;
    leftMotor = lm;
  }
  void Update(int dt) override
  {
    bool rightOn = (*rightLineSensor).isOverLine();
    Print("right line sensor" + String(rightOn));
    bool leftOn = (*leftLineSensor).isOverLine();
     Print("left line sensor" + String(leftOn));
    int error = 0;
    if(rightOn^leftOn)//are we on an edge?
    {
      //which edge are we on? keep track of that
      if (rightOn)
      {
        sideWeAreOn = right;
      } else
      {
        sideWeAreOn = left;
      }
    }
    if(!rightOn && !leftOn)//are both sensors off the line?
    {
      error = -sideWeAreOn;//set error appropriately
    }
  
    
    if (rightOn && leftOn)//are both sensors on the line?
    {
      error = sideWeAreOn;//set error appropriately
    }
    int delta = error*kLineFollow;
    if (abs(delta)>lineFollowMaxDelta)
    {
      delta = lineFollowMaxDelta*sign(delta);
    }
    if(hasControl)
    {
      setVel(lineFollowDefaultVel-abs(delta/2));
      setDelta(delta);
    }
  }
  void setVel(int vel)
  {
    //sets average velocity of robot
    (*rightMotor).setVel(vel);
    (*leftMotor).setVel(vel);
  }
  
  void setDelta(int delta)
  {
    //sets difference in velocity between the motors
    (*rightMotor).setDelta(delta/2);
    (*leftMotor).setDelta(-delta/2);
  }
  void applyControl(bool state)
  {
    hasControl = state;
  }
  void setSide(int side)
  {
    sideWeAreOn = side;
  }
};
#endif
