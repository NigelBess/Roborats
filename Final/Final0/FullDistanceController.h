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
  
};
#endif
