#ifndef GameEngine_H
#define GameEngine_H
#include "GameObject.h"
class GameEngine
{
  private:
  GameObject **objects;
  GameObject *debugger;
  unsigned long int thisLoopTime;
  unsigned long int lastLoopTime;
  uint8_t refreshTime = 10;//ms  
  int numObjects;
  public:
  GameEngine(){}
  void Initialize(int quantity, GameObject *o[], GameObject *d)
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
      if((*objects[i]).enabled)
      {
        (*objects[i]).BaseUpdate(dt);
        (*objects[i]).Update(dt);
      }
    }
    (*debugger).Update(dt);
    delay(refreshTime);

  }
  int getNumObjects()
  {
    return numObjects;
  }

};
#endif
