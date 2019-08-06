

//header files for classes used in this program
#include "Debugger.h"
#include "GameEngine.h"
#include "DCMotor.h"
#include "RangeSensor.h"
#include "WallFollower.h"



//NOTE: #define statements are used intentionally instead of constants in order
//to clear up memory for global variables

//pinouts
#define rightEncoderPin 2
#define leftEncoderPin 3
#define rightRangeSensorPin A0
#define leftRangeSensorPin A1
#define photoResPin A4
#define potPin A5


//left / right to integer
#define right 1
#define left -1

const int photoResThreshold = 250;//analog read values lower than this from the 
                                  // photo resistor will activate the robot

#define debugMode true //do we want to print to the serial monitor for debugging?
#define waitForStartLight true // do we want to wait for the start light?

//This code runs on a game engine framework.
//Most objects inherit from GameObject, which has two important methods: Awake() and Update()
//Awake() is called during setup, and Update() is called once per loop.
//A GameEngine object has references to all the GameObjects, and calls their Awake() and Update
//methods appropriately


//DC Motor GameObjects
DCMotor rightMotor(3);
DCMotor leftMotor(4);

//Range sensor GameObjects
RangeSensor rightRangeSensor(rightRangeSensorPin);
RangeSensor leftRangeSensor(leftRangeSensorPin);

//Wall Follower GameObject
WallFollower wallFollower(&rightMotor,&leftMotor);


//debugger is a GameObject used for debugging using the serial monitor
Debugger debug(debugMode);

//instantiate a GameEngine object
GameEngine engine;

void setup() 
{
  
  //start the serial monitor
  Serial.begin(9600);

  //array of references to game objects
  GameObject* objects[] = {&rightMotor,&leftMotor, &rightRangeSensor,&leftRangeSensor,
  &wallFollower};

  
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
  //calls awake on all gameobjects
  engine.Awake();

  wallFollower.enabled = true;
  wallFollower.setDirection(right);
                                                                                              

}

void loop() 
{
  //runs the game engine framework
  engine.Update();
}
