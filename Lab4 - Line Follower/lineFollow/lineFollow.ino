
#include "Motor.h"

#define debug true //do we want to print to the serial monitor?

//pinouts
#define irSensorPin A0
#define potPin A1

#define left -1;
#define right 1;

//create custom motor objects for both motors
//Motor(pin, reversePolarity?)
Motor leftMotor(1,true);
Motor rightMotor(2,true);

int defaultSpeed = 255;
int turnSpeed = 300;
unsigned long int turnTime = 50;//ms per step
unsigned long int turnEndTime = 0;
unsigned long int numSteps;
int lastDirection;
int pauseTime = 100;
bool wasOnLineLastLoop = false;
int resetTurnTime = 400;
unsigned long int centeringStartTime = 0;
const int maxCenteringTime = 1000;

int irSenseThreshold = 700;//value greater than this means we are over line

//how often do we want to print while we are in debug mode?
const int serialRefreshTime = 1000;//ms
unsigned long int nextPrintTime = 0;//ms
unsigned long int thisLoopTime = 0;

bool printThisFrame;
enum State
{
  searching = 0,
  following,
  centering,
};
State state = searching;

void resetSearchVars()
{
  numSteps = 0;
  lastDirection = left;
  turnEndTime = 0; 
}
void setVel(int vel)
{
  //sets average velocity of robot
  //printValue(vel);
  rightMotor.setVel(vel);
  leftMotor.setVel(vel);
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

  resetSearchVars();
  state = following;
}

void loop()
{
  thisLoopTime = millis();
  printThisFrame = false;
  switch (state)
  {
    case following:
      if (isOverLine())
      {
        resetSearchVars();
        moveForward();
        break;
      }
      state = searching;
      break;
    case searching:
      if (isOverLine())
      {
        state = centering;
        centeringStartTime = thisLoopTime;
        break;
      }
       if(thisLoopTime>turnEndTime)
      {
        turn(-lastDirection);
        lastDirection = -lastDirection;
        numSteps++;
        turnEndTime = thisLoopTime + numSteps*turnTime;
      }
    case centering:
      if (!isOverLine()|| thisLoopTime > (centeringStartTime + maxCenteringTime))
      {
        int turnBackTime = (thisLoopTime - centeringStartTime)/2;
        turn(-lastDirection);
        delay(turnBackTime);
        state = following;
      }
  }
  printValue(int(state));
}

bool isOverLine()
{
  //return analogRead(irSensorPin)>1.5*analogRead(potPin);
  return analogRead(irSensorPin)>irSenseThreshold;
}


void setDelta(float delta)
{
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

