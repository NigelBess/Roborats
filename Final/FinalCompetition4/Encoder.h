#ifndef Encoder_H
#define Encoder_H
class Encoder : public GameObject
{
  protected:
  int count;//encoder count
  uint8_t pin;
  short int direction = 1;//should be 1 or -1, depending on the direction we expect to turn
  short int stopTimeThreshold = 1000;//how long until we think we are stuck
  int lastCount;//what was the count at the start of last loop
  unsigned long int stopTime = 0;//time at which the encoder last stopped
  bool stopped;//are we stopped?
  
  public:
  Encoder(int pinNum)
  {
    pin = pinNum;
  }
  void Awake() override
  {
    //setup pin
    pinMode(pin,INPUT_PULLUP);
  }
  void countTick()
  {
    //interrupt function
    count += direction;
  }
  void setDirection(int newDirection)
  {
    //setter for direction. maps input to 1 or -1
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
    if (stopTime == 0)//for the first loop, reset the stop time
    {
      stopTime = millis();
    }
    
    if (count!=lastCount)
    {
      //if the encoder moved we arent stopped
      stopTime = millis();
      stopped = false;
    } else
    {
      if (!stopped)
      {
        //check if enough time has passed to declare the encoder stopped
        if ((millis()-stopTime)>stopTimeThreshold)
        {
          stopped = true;
        }
      }
    }
    lastCount = count;
  }
  void reset(int value)
  //resets encoder count to value
  {
    count = value;
  }
  void reset()
  {
    //by default resets to zero
    reset(0);
  }
  int getCount()
  {
    //public getter
    return count;
  }
  void printCount()
  {
    //for debugging
    Print("Encoder at Pin : " + String(pin));
    Print(count);
  }
  bool isStopped()
  {
    //public getter
    return stopped;
  }

};
#endif
