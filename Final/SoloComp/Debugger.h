#ifndef Debugger_H
#define Debugger_H

//The debugger is for serial monitor printing during Update().
//If Update happens in fast succession (as it should) then we will print way too fast to the serial monitor
//this object throttles the print rate

#include "GameObject.h"

class Debugger : public GameObject
{
  private:
    unsigned long int nextPrintTime = 0;//next millis() at which we will print
    bool printedThisLoop = false;//did we already print this loop?
    bool on;//if not on, dont print at all
    short int refreshTime = 500;//how often we want to print, in ms
  public:
  
  Debugger(bool debugMode)
  {
   on = debugMode;
  }
  void Awake() override
  {
    //lets us know that the debugger is working
    printedThisLoop = true;
    Print("Debugger is on");
  }
  
  void Update(int dt) override
  {
    //reset whether we have printed this loop
    printedThisLoop = false;
  }
  
  void Print(String msg) override
  {
    if (!on)
    {
      //dont print if not on
      return;
    }

      if (!printedThisLoop)//if we havent already printed this loop, check if its time to print again
      {
        if(millis()<nextPrintTime)
        {
          return;
        }
        //print empty line to separate output by the time it was printed
        Serial.println("");
      }
    printedThisLoop = true;//keep track that we already printed
    nextPrintTime = millis()+refreshTime;//take note of the next time to print (other than this loop)
    Serial.println(msg);//print the message
  }
  void setRefreshTime(int ms)
  {
    //public setter
    refreshTime = ms;
  }
};
#endif
