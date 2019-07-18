#include "ServoObj.h"
#include "Debugger.h"
#include "GameEngine.h"

#define servoPin 9
#define debugMode true

ServoObj servoObj(servoPin);
Debugger debugger(debugMode);
GameEngine engine;

void setup() 
{
  GameObject *objects[] = {&servoObj};
  engine.Initialize(sizeof(objects)/sizeof(objects[0]),objects,&debugger);
  engine.Awake();
}

void loop() 
{
  engine.Update();
}
