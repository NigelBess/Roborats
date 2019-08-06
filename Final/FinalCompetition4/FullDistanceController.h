#ifndef FullDistanceController_H
#define FullDistanceController_H
#include "DistanceController.h"

//This one is NOT a game object. It just has references to the distance controllers and acts as an intermediate object for sending commands to both controllers at a time


#define wheelTrack 0.17//distance between rear wheels in meters
#define mainGain 1500.0//gain when driving straight
#define turnGain 30000.0//gain when turning
#define turningCalibrationRatio 1.0//used to fine tune turn amount

class FullDistanceController
{
  protected :
  //references to the distance controllers
  DistanceController* right;
  DistanceController* left;
 
    
  
  public:
  FullDistanceController(DistanceController* r, DistanceController* l)
  {
    right = r;
    left = l;
  }
  void setTicksPerMeter(float tpm)
  {
    //sets distance calibration for both controllers
    (*right).setTicksPerMeter(tpm);
    (*left).setTicksPerMeter(tpm);
  }
  void setEnabled(bool state)
  {
    //sets enabled for both controllers at a time
    (*right).enabled = state;
    (*left).enabled = state;
  }
  void go(float distance)
  {
    setGain(mainGain);//set gain to straight line gain
    setEnabled(true);//enable the controllers
    //make them go the same distance
    (*right).go(distance);
    (*left).go(distance);
  }
  void setGain(float gain)
  {
    //sets gain for both at a time
    (*right).setGain(gain);
    (*left).setGain(gain);
  }
  void turn(float degree, int direction)
  {
    //turns the robot by defined degrees in defined direction (-1 or 1)
    setGain(turnGain);//sets appropriate gain
    setEnabled(true);//enable the controllers
    float distance = (degree/360.0)*3.14*wheelTrack*turningCalibrationRatio;//calculate how far each wheel needs to drive to turn desired degrees

    //make the controllers go opposite directions for the correct distance
    (*right).go(-direction*distance);
    (*left).go(direction*distance);
  }
  void turnAround(int direction)
  {
    //turns the robot mostly around
    turn(120.0,direction);
  }
  bool hasArrived()
  {
    //have we arrived
    return ((*right).hasArrived() || (*left).hasArrived());
  }
  void applyControl(bool state)
  {
    //applies control depending on state to both controllers
    (*right).applyControl(state);
    (*left).applyControl(state);
  }
  bool isStuck()
  {
    //are we stuck?
    return (*right).isStuck() && (*left).isStuck();
  }
  
};
#endif
