#ifndef Debugger_H
#define Debugger_H

#include "GameObject.h"

class Debugger : public GameObject
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
  
  void Print(String msg,bool force) override
  {
    if (!on)
    {
      return;
    }
    if (!printedThisLoop && !force)
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
  void Print(String msg) override
  {
    Print(msg,false);
  }
  void setRefreshTime(int ms)
  {
    refreshTime = ms;
  }
};
#endif
