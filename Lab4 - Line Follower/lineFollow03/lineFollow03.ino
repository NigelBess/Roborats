
#include "Motor.h"

#define debug true //do we want to print to the serial monitor?

//pinouts
#define irSensorPin A0
#define switchPin A1

#define left 1
#define right -1

//create custom motor objects for both motors
//Motor(pin, reversePolarity?)
Motor leftMotor(3,true);
Motor rightMotor(1,true);


int direction = left;//direction to turn when not over line
int turnVel = 50;//robot's average translational velocity when turning
int turnSpeed = (255-abs(turnVel))*2;//difference in speed between motors while turning

int irSenseThreshold = 500;//value greater than this means we are over line

//how often do we want to print while we are in debug mode?
const int serialRefreshTime = 1000;//ms

//time of next print
unsigned long int nextPrintTime = 0;//ms

//time that current loop started
unsigned long int thisLoopTime = 0;//ms

//stores if we printed already this loop 
bool printThisFrame;

void setVel(int vel)
{
  //sets average velocity of robot
  rightMotor.setVel(vel);
  leftMotor.setVel(vel);
}

void setup()
{
  Serial.begin(9600);//setup serial port
  
  //setup pins
  pinMode(irSensorPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
}

void loop()
{
  //store the time that the loop started (used for making sure we don't print to much)
  //also important for measuring time between loops if we want to do calculus stuff (this program doesn't do that though)
  thisLoopTime = millis();

  //reset printThisFrame at start of loop
  printThisFrame = false;

  //did the switch get bumped?
  if(!digitalRead(switchPin))
  {
    //if so, turn around
    turnAround();
    return;
  }

  //are we on the line?
  if (!isOverLine())
  {
    //if so turn towards line
   turn(direction); 
  } else
  {
    //otherwise turn away from line
    turn(-direction);
  }
}

bool isOverLine()
{
  //measures reading from IR sensor. If it's above a threshold value return true
  int reading = analogRead(irSensorPin);
    printValue(reading);//used for debugging
  return reading>irSenseThreshold;

}


void setDelta(float delta)
{
  //distribute the speed delta evenly accross the two motors
  rightMotor.setDelta(delta/2);
  leftMotor.setDelta(-delta/2);
}
void turn(int direction)
{
  setVel(turnVel);//set average speed
  setDelta(direction*turnSpeed);//set difference in speed between motors
}

void printValue(String value)
{
  if(!printThisFrame)//if we already printed this loop, we can keep printing
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
  //overload for ints
  printValue(String(value));
}

void turnAround()
{
  //turns the robot around 180* (estimated, open loop)
  setVel(0);//stop the bot
  setDelta(direction*255*2);//turn
  delay(700);//wait until we have probably reached 180.
}

