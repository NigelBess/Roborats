
#include "Motor.h"

#define debug true //do we want to print to the serial monitor?

//pinouts
#define irSensorPin A0
#define potPin A1

#define left 1
#define right -1

//create custom motor objects for both motors
//Motor(pin, reversePolarity?)
Motor leftMotor(3,true);
Motor rightMotor(1,true);

int error = 0;
float k;
int maxDelta = 300;
int maxSpeed = 250;
int maxError;
int maxErrorTime = 300;
int refreshTime = 10;
unsigned long int startLineTime;
unsigned long int lineLeaveTime;
int maxSearchTime = 400;
int onLineTimeThreshold = 100;
int direction  = 1;
bool didChangeDirection = false;

int defaultSpeed = 150;
int turnSpeed = 400;
int slightTurnDelta = 150;
//unsigned long int turnAroundEndTime = 0;
//int turnAroundTime;

int irSenseThreshold = 500;//value greater than this means we are over line

//how often do we want to print while we are in debug mode?
const int serialRefreshTime = 1000;//ms
unsigned long int nextPrintTime = 0;//ms
unsigned long int thisLoopTime = 0;

bool printThisFrame;
enum State
{
  following = 0,
  turningAround,
};
State state = following;

void setVel(int vel)
{
  //sets average velocity of robot
  //printValue(vel);
  rightMotor.setVel(vel);
  leftMotor.setVel(vel);
}
void moveForwardAndTurnSlightly(int direction)
{
  setVel(defaultSpeed-abs(slightTurnDelta/2));
  setDelta(direction * slightTurnDelta);
}
void moveForward()
{
  setVel(defaultSpeed);
  setDelta(0);
}

void setup()
{
  Serial.begin(9600);//setup serial port
  
  //setup pins
  pinMode(irSensorPin, INPUT_PULLUP);
  pinMode(potPin, INPUT_PULLUP);
 k = float(refreshTime)/float(maxErrorTime)*maxDelta;
}

void loop()
{
  thisLoopTime = millis();
  printThisFrame = false;
  if (!isOverLine())
  {
    if (error<=0)
    {
      lineLeaveTime = millis();
    }
    if (!didChangeDirection && millis()>(lineLeaveTime+maxSearchTime))
    {
      didChangeDirection = true;
      direction = -direction;
    }
    error++;
    error = max(error,0);
  } else
  {
    didChangeDirection = false;
    if (error>0)
    {
      startLineTime = millis();
    }
    //error-=3;
    error = min(error,0);
    if(millis()>(startLineTime+onLineTimeThreshold))
    {
      error = 0; 
    }
  }
  error = min(maxErrorTime/refreshTime,error);
  if (error == 0)
  {
    setDelta(0);
    setVel(maxSpeed);
  }else
  {
    setDelta(direction*k*error);
  }
  
  printValue(k*error);
  delay(refreshTime);
}

bool isOverLine()
{
  //return analogRead(irSensorPin)>1.5*analogRead(potPin);
  int reading = analogRead(irSensorPin);
    printValue(reading);
  return reading>irSenseThreshold;

}


void setDelta(float delta)
{
  delta = min(delta,maxDelta);
  setVel(defaultSpeed - abs(delta/2)); 
  //distribute the speed delta evenly accross the two motors
  rightMotor.setDelta(delta/2);
  leftMotor.setDelta(-delta/2);
}
void turn(int direction)
{
  setVel(0);
  setDelta(direction*turnSpeed);
}

void printValue(String value)
{
  if(!printThisFrame)
  {
    //is it time to print again? if not return
    if (!debug || (thisLoopTime<nextPrintTime))
    {
      return;
    }
  }
  //if we made it this far, it's time to print
  Serial.println(value); 
  nextPrintTime = thisLoopTime+serialRefreshTime;
  printThisFrame = true;
}
void printValue(int value)
{
  printValue(String(value));
}
void halt()
{
  setVel(0);
  setDelta(0);
}
void pause(int time)
{
  halt();
  delay(time);
}

