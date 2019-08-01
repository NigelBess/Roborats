#ifndef Debugger_H
#define Debugger_H

#include "GameObject.h"

class Debugger : public GameObject
{
  private:
    unsigned long int nextPrintTime = 0;
    bool printedThisLoop = false;
    bool on;
    short int refreshTime = 500;
  public:
  Debugger(bool debugMode)
  {
   on = debugMode;
  }
  void Awake() override
  {
    Serial.begin(9600);
    printedThisLoop = true;
    Print("Debugger is on");
  }
  void Update(int dt) override
  {
    printedThisLoop = false;
  }
  
  void Print(String msg) override
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
    printedThisLoop = true;
    nextPrintTime = millis()+refreshTime;
    Serial.println(msg); 
  }
  void setRefreshTime(int ms)
  {
    refreshTime = ms;
  }
};
#endif
