#ifndef DistanceController_H
#define DistanceController_H
#include "DCMotor.h"
#include "Encoder.h"

//This is a proportional controller for making a single motor turn a set distance


#define encoderTicksPerMeter 139//emprirical calibration constant for encoder ticks per meter traveled by the robot
#define maxOutput 255//max allowable velocity output to the motor

class DistanceController : public GameObject
{
  protected :
    //reference to the motor and appropriate encoder
    DCMotor* motor;
    Encoder* encoder;
    
    float k = 900;//gain

    //target position
    float targetPos = 0;

    //prevents us from having to convert from ticks per meter
    float metersPerTick = 0;

    //if we are within this distance, we have arrived
    float arrivedThreshold = 0.03;

    //are we at the desitination
    bool arrived = true;

    //if not active, dont affect motor speeds (but still update so we can check if we have arrived)
    //this is used to verify distance traveled by the wall follower while not interering with its control of the motors
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
    //public getter
    return arrived;
  }
  void setGain(float gain)
  {
    //public setter
    k = gain;
  }
  void Update(int dt) override
  {
    float currentPos = float((*encoder).getCount())*metersPerTick;//read the encoder
    float error = targetPos - currentPos;//calculate error
    
    //check if we have arrived
    if (!arrived)
    {
      arrived = (abs(error)<arrivedThreshold);
    }
    
    //proportional control
    int output = int(error*k);

    //saturate output
    if (abs(output) > maxOutput)
    {
      output = maxOutput*sign(output);
    }

    //if the controller is active, affect the motor's velocity
    if (active)
    {
      (*motor).setTargetVel(output);
    } 
  }
  void setTicksPerMeter(float tpm)
  {
    //converts to meters per tick
    metersPerTick = 1.0/tpm;
  }
  
  void go(float distance)
  {
    //sets a new target position. negative distance is backward
    Serial.println("new target distance: " + String(distance));
    (*encoder).reset();
    targetPos = distance;
    arrived = false;
  }
  void applyControl(bool state)
  {
    //public setter
    active = state;
  }
  bool isStuck()
  {
    //checks if the encoder hasn't moved in a while
    return(!arrived && (*encoder).isStopped());
  }
  
};
#endif
