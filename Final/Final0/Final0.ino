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
#include "LineSensor.h"
#include "Grabber.h"
#include "MainStateMachine.h"

//pinouts
#define rightEncoderPin 2
#define leftEncoderPin 3
#define rightRangeSensorPin A0
#define leftRangeSensorPin A1
#define rightLineSensorPin A2
#define leftLineSensorPin A3
#define armServoPin 10
#define handServoPin 9

#define right 1
#define left -1


#define debugMode true

Encoder rightEncoder(rightEncoderPin);
Encoder leftEncoder(leftEncoderPin);
LineSensor rightLineSensor(rightLineSensorPin);
LineSensor leftLineSensor(leftLineSensorPin);
DCMotor rightMotor(3);
DCMotor leftMotor(4);
ServoObj armServo(armServoPin);
ServoObj handServo(handServoPin);
DistanceController rightDistanceController(&rightMotor,&rightEncoder);
DistanceController leftDistanceController(&leftMotor,&leftEncoder);
FullDistanceController distControl(&rightDistanceController,&leftDistanceController);
RangeSensor rightRangeSensor(rightRangeSensorPin);
RangeSensor leftRangeSensor(leftRangeSensorPin);
WallFollower wallFollower(&rightMotor,&leftMotor);
SideMover sideMover(&rightMotor,&leftMotor,&rightRangeSensor,&leftRangeSensor,&distControl,&wallFollower,&rightLineSensor);
Grabber grabber(&armServo,&handServo);
MainStateMachine robot(&grabber);


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
  GameObject *objects[] = {&rightEncoder,&leftEncoder,&rightMotor,&leftMotor,
  &rightDistanceController,&leftDistanceController,&rightRangeSensor,&leftRangeSensor,
  &wallFollower,&sideMover,&rightLineSensor,&leftLineSensor,&armServo,&handServo,&grabber,&robot};

  //pass references to game engine
  engine.Initialize(sizeof(objects)/sizeof(objects[0]),objects,&debug);

  //calls awake on all gameobjects
  engine.Awake();

  
  robot.setMover(&sideMover);
  //x       robot.collectCheese();
  //grabber.grabCheese();
  armServo.setTargetPos(180);
  handServo.setTargetPos(90);
}

void loop() 
{
  engine.Update();
  if(armServo.hasArrived())
  {
    armServo.setTargetPos(0);
  }  
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
