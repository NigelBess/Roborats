#ifndef Encoder_H
#define Encoder_H
class Encoder : public GameObject
{
  protected:
  int count;
  uint8_t pin;
  short int direction = 1;
  short int stopTimeThreshold = 1000;
  int lastCount;
  unsigned long int stopTime = 0;
  bool stopped;
  
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
  void Update(int dt) override
  {
    if (stopTime == 0)
    {
      stopTime = millis();
    }
    if (count!=lastCount)
    {
      stopTime = millis();
      stopped = false;
    } else
    {
      if (!stopped)
      {
        if ((millis()-stopTime)>stopTimeThreshold)
        {
          stopped = true;
        }
      }
    }
    lastCount = count;
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
  bool isStopped()
  {
    return stopped;
  }

};
#endif
