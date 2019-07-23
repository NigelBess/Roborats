#ifndef LineSensor_H
#define LineSensor_H
class LineSensor: public GameObject
{
  protected:
    int pin;
    const int threshold = 
  public:
  LineSensor(int pinNum)
  {
    pin = pinNum;
  }
  void Awake() override
  {
    pinMode(pin,INPUT_PULLUP);
  }
  float getDistance()
  {
    //uses calibration table to interpolate from the analog signal
    return interpolateFromCalibration(analogRead(pin));
  }
  
};
#endif
