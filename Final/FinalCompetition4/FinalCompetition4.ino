

//header files for classes used in this program
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



//NOTE: #define statements are used intentionally instead of constants in order
//to clear up memory for global variables

//pinouts
#define rightEncoderPin 2
#define leftEncoderPin 3
#define rightRangeSensorPin A0
#define leftRangeSensorPin A1
#define rightLineSensorPin A2
#define leftLineSensorPin A3
#define photoResPin A4
#define potPin A5
#define armServoPin 9
#define handServoPin 10

//left / right to integer
#define right 1
#define left -1

const int photoResThreshold = 250;//analog read values lower than this from the 
                                  // photo resistor will activate the robot

//the potentiometer is divided into three sections to tell the robot
//where the golden cheese is. The constants below define the borders of those regions
//(as analog read values)
const int leftPotReading = 750;
const int rightPotReading = 160;

#define debugMode true //do we want to print to the serial monitor for debugging?
#define waitForStartLight true // do we want to wait for the start light?

//This code runs on a game engine framework.
//Most objects inherit from GameObject, which has two important methods: Awake() and Update()
//Awake() is called during setup, and Update() is called once per loop.
//A GameEngine object has references to all the GameObjects, and calls their Awake() and Update
//methods appropriately


//Encoder GameObjects
Encoder rightEncoder(rightEncoderPin);
Encoder leftEncoder(leftEncoderPin);

//LineSensor GameObjects
LineSensor rightLineSensor(rightLineSensorPin);
LineSensor leftLineSensor(leftLineSensorPin);

//DC Motor GameObjects
DCMotor rightMotor(3);
DCMotor leftMotor(4);

//Servo Motor GameObjects
ServoObj armServo(armServoPin);
ServoObj handServo(handServoPin);

//Proportional distance controller GameObjects, one per wheel
DistanceController rightDistanceController(&rightMotor,&rightEncoder);
DistanceController leftDistanceController(&leftMotor,&leftEncoder);

//The full distance controller is not a game object. It is used for sending commands to
//both distance controllers.
FullDistanceController distControl(&rightDistanceController,&leftDistanceController);

//Range sensor GameObjects
RangeSensor rightRangeSensor(rightRangeSensorPin);
RangeSensor leftRangeSensor(leftRangeSensorPin);

//Wall Follower GameObject
WallFollower wallFollower(&rightMotor,&leftMotor);

//side mover is a GameObject that handles higher level control of the motion of the robot
SideMover sideMover(&rightMotor,&leftMotor,&rightRangeSensor,&leftRangeSensor,&distControl,&wallFollower,&rightLineSensor,&leftLineSensor);

//grabber is a GameObject that handles higher level control of the grabbing arm
Grabber grabber(&armServo,&handServo);

//the MainStateMachine is a GameObject that handles commands to the side mover
// and grabber, at the highest level of abstraction
MainStateMachine robot(&grabber);

//debugger is a GameObject used for debugging using the serial monitor
Debugger debug(debugMode);

//instantiate a GameEngine object
GameEngine engine;

void setup() 
{
  //attach interrupts in main function because arduino doesn't let you pass method pointers belonging to instances
  //(instead pass wrapper functions)
  attachEncoderInterrupts();
  
  //start the serial monitor
  Serial.begin(9600);
  
  //pass references to encoders to the motors. The motors set the direction of encoder counts to match the direction of motor rotation
  rightMotor.setEncoder(&rightEncoder);
  leftMotor.setEncoder(&leftEncoder);

  //array of references to game objects
  GameObject* objects[] = {&rightEncoder,&leftEncoder,&rightMotor,&leftMotor,
  &rightDistanceController,&leftDistanceController,&rightRangeSensor,&leftRangeSensor,
  &wallFollower,&sideMover,&rightLineSensor,&leftLineSensor,&armServo,&handServo,
  &grabber,&robot};

  
  //pass references to GameEngine
  engine.Initialize(sizeof(objects)/sizeof(objects[0]),objects,&debug);

  //setup the startlight pin
    pinMode(photoResPin, INPUT_PULLUP);

    //if we want to wait for the start light, stall until it is activated
    if(waitForStartLight)
    {
       while(analogRead(photoResPin)>photoResThreshold)
      {
        //print statement for debugging the start light photoresistor
        Serial.println(analogRead(photoResPin));
      }
   }

  //setup the potentiometer
  pinMode(potPin, INPUT);
  
  //get the value of the potentiometer
  int potReading = analogRead(potPin);

  //which third is the potentiometer aligned to?
  if(potReading>leftPotReading) 
  {
    //if its on the left, get the left hanging cheese
    sideMover.setSide(left);
  } else if (potReading<rightPotReading)   
  {
    //if its on the right, get the right hanging cheese
    sideMover.setSide(right);
  } else
  {
    //if the potentiometer is in the center get the right cheese first
    sideMover.setSide(right);
    
    robot.getBothCheeses();//tell the robot that we are going to get both cheeses
  }
  //calls awake on all gameobjects
  engine.Awake();

  //disable all controllers by default
  wallFollower.enabled = false;
  distControl.setEnabled(false);

  //pass a reference to the side mover to the robot
  //this could have been done in the constructor, but for a while 
  //we had a different mover object for the center cheese, and that would get passed
  robot.setMover(&sideMover);

  //start collecting cheese
  robot.collectCheese();                                                                                                 

}

void loop() 
{
  //runs the game engine framework
  engine.Update();
}


















//encoder stuff
void attachEncoderInterrupts()
{
  attachInterrupt(digitalPinToInterrupt(rightEncoderPin),rightCountTick,FALLING);
  attachInterrupt(digitalPinToInterrupt(leftEncoderPin),leftCountTick,FALLING);
}

//wrapper functions for the encoder interrupts.
//I couldn't figure out how to pass instance methods as void*
void rightCountTick()
{
  rightEncoder.countTick();
}
void leftCountTick()
{
  leftEncoder.countTick();
}
