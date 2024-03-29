#ifndef LineSensor_H
#define LineSensor_H
class LineSensor: public GameObject
{
  protected:
    int pin;
    const int threshold = 500;
  public:
  LineSensor(int pinNum)
  {
    pin = pinNum;
  }
  void Awake() override
  {
    pinMode(pin,INPUT_PULLUP);
  }
  bool isOverLine()
  {
    return (analogRead(pin)>threshold);
  }
  
};
#endif
