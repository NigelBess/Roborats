#include "ServoObj.h"
#include "Debugger.h"
#include "GameEngine.h"
#include "Encoder.h"
#include "DCMotor.h"
#include "DistanceController.h"
#include "FullDistanceController.h"

#define servoPin 10
#define rightEncoderPin 2
#define leftEncoderPin 3
#define debugMode true

#define distanceControllerGain 1000
#define encoderTicksPerMeter 100
#define motorAcceleration 510 // counts per second

ServoObj servo(servoPin);
Encoder rightEncoder(rightEncoderPin);
Encoder leftEncoder(leftEncoderPin);
DCMotor rightMotor(3);
DCMotor leftMotor(4);
DistanceController rightDistanceController(&rightMotor,&rightEncoder,distanceControllerGain);
DistanceController leftDistanceController(&leftMotor,&leftEncoder,distanceControllerGain);
FullDistanceController distControl(&rightDistanceController,&leftDistanceController);
Debugger debug(debugMode);
GameEngine engine;

void setup() 
{
  rightMotor.setEncoder(&rightEncoder);
  leftMotor.setEncoder(&leftEncoder);
  GameObject *objects[] = {&servo,&rightEncoder,&leftEncoder,&rightMotor,&leftMotor,&rightDistanceController,&leftDistanceController};
  engine.Initialize(sizeof(objects)/sizeof(objects[0]),objects,&debug);
  engine.Awake();
  attachInterrupt(digitalPinToInterrupt(rightEncoderPin),rightCountTick,FALLING);
  attachInterrupt(digitalPinToInterrupt(leftEncoderPin),leftCountTick,FALLING);

  distControl.setTicksPerMeter(encoderTicksPerMeter);
  distControl.setTargetPos(1);//m
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

void rightCountTick()
{
  rightEncoder.countTick();
}
void leftCountTick()
{
  leftEncoder.countTick();
}
