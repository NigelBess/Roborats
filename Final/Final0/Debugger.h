#ifndef Debugger_H
#define Debugger_H

class Debugger
{
  private:
    unsigned long int nextPrintTime = 0;
    bool printedThisLoop = false;
    bool on;
    int refreshTime = 500;
  public:
  Debugger(bool debugMode)
  {
   on = debugMode;
  }
  void Awake()
  {
    Serial.begin(9600);
    printedThisLoop = true;
    Print("Debugger is on");
  }
  void Update(int dt)
  {
    printedThisLoop = false;
  }
  void Print(String msg)
  {
    if (!on)
    {
      return;
    }
    if (!printedThisLoop)
    {
      if(millis()<nextPrintTime)
      {
        return;
      }
      Serial.println("");
    }
    nextPrintTime = millis()+refreshTime;
    Serial.println(msg); 
  }
  void Print(int msg)
  {
    Print(String(msg));
  }
  void Print(float msg)
  {
    Print(String(msg));
  }
  void setRefreshTime(int ms)
  {
    refreshTime = ms;
  }
};
#endif
