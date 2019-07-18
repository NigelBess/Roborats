#ifndef GameEngine_H
#define GameEngine_H
#include "GameObject.h"
//#include "Debugger.h"
class GameEngine
{
  private:
  GameObject **objects;
  Debugger *debugger;
  unsigned long int thisLoopTime;
  unsigned long int lastLoopTime;
  uint8_t refreshTime = 100;//ms  
  int numObjects;
  public:
  GameEngine(){}
  void Initialize(int quantity, GameObject *o[], Debugger *d)
  {
    objects = new GameObject*[quantity];
    for (int i = 0; i < quantity; i++)
    {
      objects[i] = o[i];
      (*objects[i]).setDebugger(d);
    }
    numObjects = quantity;
    debugger = d;
  }
  void Awake()
  {
    (*debugger).Awake();
    for (int i = 0; i < numObjects; i++)
    {
      (*objects[i]).Awake();
    }
  }
  void Update()
  {
    thisLoopTime = millis();
    int dt = (int) (thisLoopTime - lastLoopTime);
   
    lastLoopTime = thisLoopTime;
    for (int i = 0; i < numObjects; i++)
    {
      (*objects[i]).Update(dt);
    }
    (*debugger).Update(dt);
    delay(refreshTime-(millis()-thisLoopTime));
  }
  int getNumObjects()
  {
    return numObjects;
  }

};
#endif
