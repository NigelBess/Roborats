#ifndef DistanceController_H
#define DistanceController_H
#include "DCMotor.h"
#include "Encoder.h"

class DistanceController : public GameObject
{
  protected :
    DCMotor* motor;
    Encoder* encoder;
    float k;
    float targetPos = 0;
    float metersPerTick = 0;
    const int maxOutput= 255;
    float arrivedThreshold = 0.01;
    bool arrived;
    
  
  public:
  DistanceController(DCMotor* mot, Encoder* enc, float gain,float tpm)
  {
    motor = mot;
    encoder = enc;
    k = gain;
    setTicksPerMeter(tpm);
  }
  bool hasArrived()
  {
    return arrived;
  }
  void setTargetPos(float target)
  {
    targetPos = target;
  }
  void Update(int dt) override
  {
    float currentPos = float((*encoder).getCount())*metersPerTick;
    float error = targetPos - currentPos;
    arrived = (error<arrivedThreshold);
    int output = int(error*k);
    if (abs(output) > maxOutput)
    {
      output = maxOutput*sign(output);
    }
    Print("controller output");
    Print(output);
    (*motor).setTargetVel(output); 
  }
  void setTicksPerMeter(float tpm)
  {
    metersPerTick = 1/tpm;
  }
  void go(float distance)
  {
    (*encoder).reset();
    setTargetPos(distance);
  }
  
  
};
#endif
