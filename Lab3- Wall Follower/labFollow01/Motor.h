#ifndef Motor_H
#define Motor_H
#include <AFMotor.h>
class Motor
{
  private:
  AF_DCMotor *motorObj = 0;
  int nominalVel = 0;
  float delta = 0;
  void applyVel()
  {
    int vel = nominalVel + int(delta);
    (*motorObj).setSpeed(min(abs(vel),255));
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
    nominalVel = vel;
    applyVel();
  }
  void setDelta(float value)
  {
    delta = value;
    applyVel();
  }
  int getVel()
  {
    return nominalVel;
  }
  void run(int direction)
  {
    (*motorObj).run(direction);
  }
};
#endif
