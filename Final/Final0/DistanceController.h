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
    
  
  public:
  DistanceController(DCMotor* mot, Encoder* enc, float gain)
  {
    motor = mot;
    encoder = enc;
    k = gain;
  }
  void setTargetPos(float target)
  {
    targetPos = target;
  }
  void Update(int dt) override
  {
    float currentPos = float((*encoder).getCount())*metersPerTick;
    float error = targetPos - currentPos;
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
  
  
};
#endif
