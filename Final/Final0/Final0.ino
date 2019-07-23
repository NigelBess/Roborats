#include "ServoObj.h"
#include "Debugger.h"
#include "GameEngine.h"
#include "Encoder.h"
#include "DCMotor.h"
#include "DistanceController.h"
#include "FullDistanceController.h"
#include "RangeSensor.h"
#include "WallFollower.h"


//pinouts
#define servoPin 10
#define rightEncoderPin 2
#define leftEncoderPin 3
#define rightRangeSensorPin A0
#define leftRangeSensorPin A1

#define right 1
#define left -1


#define debugMode true

#define distanceControllerGain 1000
#define encoderTicksPerMeter 100
#define motorAcceleration 510 // counts per second

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
WallFollower wallFollower(&rightMotor,&leftMotor);

Debugger debug(debugMode);
GameEngine engine;

void setup() 
{
  //attach interrupts in main function because arduino doesn't let you pass method pointers belonging to instances
  //(instead pass wrapper functions)
  attachEncoderInterrupts();

  //pass references to encoders to motors. The motors set the direction of encoder counts to match the direction of motor rotation
  rightMotor.setEncoder(&rightEncoder);
  leftMotor.setEncoder(&leftEncoder);

  //array of references to game objects
  GameObject *objects[] = {&servo,&rightEncoder,&leftEncoder,&rightMotor,&leftMotor,
  &rightDistanceController,&leftDistanceController,&rightRangeSensor,&leftRangeSensor,&wallFollower};

  //pass references to game engine
  engine.Initialize(sizeof(objects)/sizeof(objects[0]),objects,&debug);

  //calls awake on all gameobjects
  engine.Awake();
  

  distControl.setTargetPos(1);//m
  wallFollower.setSensor(&rightRangeSensor);
  wallFollower.setDirection(right);
  //  servo.setAccel(100.0);
  //  servo.setVel(0);
  //  servo.setTargetVel(100.0);
  //  servo.setPos(0);
  //  servo.setTargetPos(90.0);
  //  servo.setNextTargetPos(0);
  Motor* motors[] = {&rightMotor,&leftMotor};
  for (int i = 0; i < 2; i++)
  {
    (*motors[i]).setAccel(motorAcceleration);
    (*motors[i]).setTargetVel(255);
  }
}

void loop() 
{
  engine.Update();  
}

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
