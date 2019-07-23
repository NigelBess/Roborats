#ifndef FullDistanceController_H
#define FullDistanceController_H
#include "DistanceController.h"

class FullDistanceController
{
  protected :
  DistanceController* right;
  DistanceController* left;
  
    
  
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
     setEnabled(true);
    (*right).go(distance);
    (*left).go(distance);
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
