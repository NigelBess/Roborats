#ifndef DistanceController_H
#define DistanceController_H
#include "DCMotor.h"
#include "Encoder.h"

#define encoderTicksPerMeter 139
#define maxOutput 255

class DistanceController : public GameObject
{
  protected :
    DCMotor* motor;
    Encoder* encoder;
    float k = 900;
    
    float targetPos = 0;
    float metersPerTick = 0;
    
    float arrivedThreshold = 0.03;
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
    Serial.println("new target distance: " + String(distance));
    (*encoder).reset();
    setTargetPos(distance);
  }
  void applyControl(bool state)
  {
    active = state;
  }
  bool isStuck()
  {
    return(!arrived && (*encoder).isStopped());
  }
  
};
#endif
