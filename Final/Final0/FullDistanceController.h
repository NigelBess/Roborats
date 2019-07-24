#ifndef FullDistanceController_H
#define FullDistanceController_H
#include "DistanceController.h"

class FullDistanceController
{
  protected :
  DistanceController* right;
  DistanceController* left;
  const float wheelTrack = 0.17;//distance between rear wheels in meters
  const float mainGain = 700;
  const float turnGain = 700;
    
  
  public:
  FullDistanceController(DistanceController* r, DistanceController* l)
  {
    right = r;
    left = l;
  }
  void setTargetPos(float target)
  {
    (*right).setTargetPos(target);
    (*left).setTargetPos(target);
  }
  void setTicksPerMeter(float tpm)
  {
    (*right).setTicksPerMeter(tpm);
    (*left).setTicksPerMeter(tpm);
  }
  void setEnabled(bool state)
  {
    (*right).enabled = state;
    (*left).enabled = state;
  }
  void go(float distance)
  {
      setGain(mainGain);
     setEnabled(true);
    (*right).go(distance);
    (*left).go(distance);
  }
  void setGain(float gain)
  {
    (*right).setGain(gain);
    (*left).setGain(gain);
  }
  void turn(float degree, int direction)
  {
    setGain(turnGain);
    setEnabled(true);
    float distance = (degree/360.0)*3.14*wheelTrack;
    (*right).go(-direction*distance);
    (*left).go(direction*distance);
  }
  void turnAround(int direction)
  {
    turn(180.0,direction);
  }
  bool hasArrived()
  {
    return ((*right).hasArrived() || (*left).hasArrived());
  }
  void applyControl(bool state)
  {
    (*right).applyControl(state);
    (*left).applyControl(state);
  }
  
};
#endif
