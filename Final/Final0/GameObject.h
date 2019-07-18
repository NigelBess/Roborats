#ifndef GameObject_H
#define GameObject_H
#include "Debugger.h"
class GameObject
{
  public:
  bool enabled;
  Debugger *debugger;
  virtual void Awake(){}
  virtual void Update(int dt){}
  void setDebugger(Debugger *d)
  {
    debugger = d;
    (*d).Print("added to gameobject");
  }
  GameObject(){}
  void Print(String msg)
  {
    (*debugger).Print(msg);
  }
  void Print(int msg)
  {
    (*debugger).Print(msg);
  }
  void Print(float msg)
  {
    (*debugger).Print(msg);
  } 
};
#endif
