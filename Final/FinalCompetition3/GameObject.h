#ifndef GameObject_H
#define GameObject_H
class GameObject
{
  protected:
  GameObject *debugger;
  
  public:
  bool enabled = true;
  virtual void Awake(){}
  virtual void Update(int dt){}
  virtual void BaseUpdate(int dt) {}
  void setDebugger(GameObject *d)
  {
    debugger = d;
    (*d).Print("added to gameobject");
  }
  GameObject(){}
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
  virtual void SetEnabled(bool state)
  {
    enabled = false;
  }
  
  
};
#endif
