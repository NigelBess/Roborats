#ifndef LineSensor_H
#define LineSensor_H

//object for line sensors

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
    //setup appropriate pin
    pinMode(pin,INPUT_PULLUP);
  }
  
  bool isOverLine()
  {
    //is the sensor over the line?
    return (analogRead(pin)>threshold);
  }
  
  void setSensitivity(int sense)
  {
    //allows you to change the threshold reading
    threshold = sense;
  }
  
};
#endif
