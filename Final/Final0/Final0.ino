#include "ServoObj.h"
#include "Debugger.h"
#include "GameEngine.h"
#include "Encoder.h"
#include "DCMotor.h"
#include "DistanceController.h"
#include "FullDistanceController.h"
#include "RangeSensor.h"
#include "WallFollower.h"
#include "SideMover.h"


//pinouts
#define servoPin 10
#define rightEncoderPin 2
#define leftEncoderPin 3
#define rightRangeSensorPin A0
#define leftRangeSensorPin A1

#define right 1
#define left -1


#define debugMode true

const int  encoderTicksPerMeter =  125;
const int distanceControllerGain = 700;
const float  wallFollowerGain = 30;

ServoObj servo(servoPin);
Encoder rightEncoder(rightEncoderPin);
Encoder leftEncoder(leftEncoderPin);
DCMotor rightMotor(3);
DCMotor leftMotor(4);
DistanceController rightDistanceController(&rightMotor,&rightEncoder,distanceControllerGain,encoderTicksPerMeter);
DistanceController leftDistanceController(&leftMotor,&leftEncoder,distanceControllerGain,encoderTicksPerMeter);
FullDistanceController distControl(&rightDistanceController,&leftDistanceController);
RangeSensor rightRangeSensor(rightRangeSensorPin);
RangeSensor leftRangeSensor(leftRangeSensorPin);
WallFollower wallFollower(&rightMotor,&leftMotor,wallFollowerGain);
SideMover sideMover(&rightMotor,&leftMotor,&rightRangeSensor,&leftRangeSensor,&distControl,&wallFollower);


Debugger debug(debugMode);
GameEngine engine;

void setup() 
{
  //attach interrupts in main function because arduino doesn't let you pass method pointers belonging to instances
  //(instead pass wrapper functions)
  attachEncoderInterrupts();

  //pass references to encoders to the motors. The motors set the direction of encoder counts to match the direction of motor rotation
  rightMotor.setEncoder(&rightEncoder);
  leftMotor.setEncoder(&leftEncoder);

  //array of references to game objects
  GameObject *objects[] = {&servo,&rightEncoder,&leftEncoder,&rightMotor,&leftMotor,
  &rightDistanceController,&leftDistanceController,&rightRangeSensor,&leftRangeSensor,
  &wallFollower,&sideMover};

  //pass references to game engine
  engine.Initialize(sizeof(objects)/sizeof(objects[0]),objects,&debug);

  //calls awake on all gameobjects
  engine.Awake();
  sideMover.goToCheese();
}

void loop() 
{
  engine.Update();  
}


















//encoder stuff
void attachEncoderInterrupts()
{
  attachInterrupt(digitalPinToInterrupt(rightEncoderPin),rightCountTick,FALLING);
  attachInterrupt(digitalPinToInterrupt(leftEncoderPin),leftCountTick,FALLING);
}
void rightCountTick()
{
  rightEncoder.countTick();
}
void leftCountTick()
{
  leftEncoder.countTick();
}
