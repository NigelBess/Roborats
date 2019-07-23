#ifndef Encoder_H
#define Encoder_H
class Encoder : public GameObject
{
  protected:
  int count;
  int pin;
  int direction = 1;
  void* countTickPtr;
  
  public:
  Encoder(int pinNum)
  {
    pin = pinNum;
  }
  void Awake() override
  {
    pinMode(pin,INPUT_PULLUP);
  }
  void countTick()
  {
    count += direction;
  }
  void setDirection(int newDirection)
  {
    if(newDirection>=0)
    {
      direction = 1;
    } else
    {
      direction = -1;
    }
  }
  void reset(int value)
  {
    count = value;
  }
  void reset()
  {
    reset(0);
  }
  int getCount()
  {
    return count;
  }
  void printCount()
  {
    Print("Encoder at Pin : " + String(pin));
    Print(count);
  }

};
#endif
