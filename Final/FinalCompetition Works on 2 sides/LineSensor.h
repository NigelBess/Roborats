#ifndef LineSensor_H
#define LineSensor_H
class LineSensor: public GameObject
{
  protected:
    uint8_t pin;
    short int threshold = 500;
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
  void setSensitivity(int sense)
  {
    threshold = sense;
  }
  
};
#endif
