#ifndef DistanceController_H
#define DistanceController_H
#include "DCMotor.h"
#include "Encoder.h"

class DistanceController : public GameObject
{
  protected :
    DCMotor* motor;
    Encoder* encoder;
    float k = 700;
    const int  encoderTicksPerMeter =  139;
    float targetPos = 0;
    float metersPerTick = 0;
    const int maxOutput= 255;
    float arrivedThreshold = 0.01;
    bool arrived = true;
    bool active = true;
    
  
  public:
  DistanceController(DCMotor* mot, Encoder* enc)
  {
    motor = mot;
    encoder = enc;
    setTicksPerMeter(encoderTicksPerMeter);
  }
  bool hasArrived()
  {
    return arrived;
  }
  void setTargetPos(float target)
  {
    targetPos = target;
    arrived = false;
  }
  void setGain(float gain)
  {
    k = gain;
  }
  void Update(int dt) override
  {
    float currentPos = float((*encoder).getCount())*metersPerTick;
    float error = targetPos - currentPos;
    if (!arrived)
    {
      arrived = (abs(error)<arrivedThreshold);
      if (arrived)
      {
        Print("currentPos: " + String(currentPos) + ", targetPos: " + String(targetPos),true);
        Print("reached destination!",true);
      }
    }
    int output = int(error*k);
    if (abs(output) > maxOutput)
    {
      output = maxOutput*sign(output);
    }
    if (active)
    {
      (*motor).setTargetVel(output);
    } 
  }
  void setTicksPerMeter(float tpm)
  {
    metersPerTick = 1.0/tpm;
  }
  void go(float distance)
  {
    Print("new target distance: " + String(distance),true);
    (*encoder).reset();
    setTargetPos(distance);
  }
  void applyControl(bool state)
  {
    active = state;
  }
  
  
};
#endif
