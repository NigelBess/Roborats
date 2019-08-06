#ifndef GameObject_H
#define GameObject_H
class GameObject
{
  protected:
  GameObject *debugger;
  
  public:
  bool enabled = true;//if enabled call update. else dont
  virtual void Awake(){}//called on setup
  virtual void Update(int dt){}//called every loop. dt is the time since last loop
  virtual void BaseUpdate(int dt) {}//also called every loop, but used for passing superclass update methods to subclasses 
  void setDebugger(GameObject *d)
  {
    //passes reference to debugger
    debugger = d;
    (*d).Print("added to gameobject");
  }
  GameObject(){}

  //built in print functions for each gameobject
  virtual void Print(String msg)
  {
    (*debugger).Print(msg);
  }
 
  virtual void Print(int msg)
  {
    Print(String(msg));
  }
  
  virtual void Print(float msg)
  {
    Print(String(msg));
  }  

  //gets sign of a float as an int
  static float sign(float var)
  {
    if (var == 0)
    {
      return 0;
    }
    if (var<0)
    {
      return -1;
    }
    return 1;
  }
  
};
#endif
