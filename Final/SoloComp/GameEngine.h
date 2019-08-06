#ifndef GameEngine_H
#define GameEngine_H
#include "GameObject.h"

//this class runs the game engine framework
class GameEngine
{
  private:
  GameObject** objects;//array of pointers to game objects
  GameObject* debugger;// reference to the debugger
  unsigned long int thisLoopTime;//when did this loop start
  unsigned long int lastLoopTime;//when did the last loop start
  uint8_t refreshTime = 1;//delay between loops in ms. A fixed delay prevents time between loops (which is an int) to reach zero 
  uint8_t numObjects;//number of gameobjects
  public:
  
  GameEngine(){}
  void Initialize(int quantity, GameObject *o[], GameObject *d)
  {
    objects = new GameObject*[quantity];//allocate array of pointers to gameobjects
    for (int i = 0; i < quantity; i++)
    {
      objects[i] = o[i];//fill in the array
      (*objects[i]).setDebugger(d);//pass reference to debugger to each gameobject
    }
    numObjects = quantity;
    debugger = d;
  }
  void Awake()
  {
    (*debugger).Awake();//call awake on the debugger before the gameobjects

    //call awake on each gameobject
    for (int i = 0; i < numObjects; i++)
    {
      (*objects[i]).Awake();
    }
  }
  void Update()
  {
    //calculate time between loops
    thisLoopTime = millis();
    int dt = (int) (thisLoopTime - lastLoopTime);
    lastLoopTime = thisLoopTime;
    
    //call each object's update and baseupdate function
    for (int i = 0; i < numObjects; i++)
    {
      if((*objects[i]).enabled)
      {
        (*objects[i]).BaseUpdate(dt);
        (*objects[i]).Update(dt);
      }
    }
    
    //update the debugger
    (*debugger).Update(dt);
    
    //make sure dt doesnt drop to zero
    delay(refreshTime);

  }

};
#endif
