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
  virtual void Print(String msg,bool force)
  {
    (*debugger).Print(msg,force);
  }
  
  virtual void Print(String msg)
  {
    Print(msg, false);
  }
 

  
  virtual void Print(int msg,bool force)
  {
    Print(String(msg),force);
  }
  virtual void Print(int msg)
  {
    Print(String(msg),false);
  }
  
  virtual void Print(float msg,bool force)
  {
    Print(String(msg),force);
  }  
  virtual void Print(float msg)
  {
    Print(String(msg),false);
  }
  
  float sign(float var)
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
