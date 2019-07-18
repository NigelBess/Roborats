
//this program uses two sensors to determine which side of the line the robot is on.
//Using this information, and a cost associated with being on the line, off the line, or on the edge,
//an error is determined. The error goes through a variation on a PI controller, to create a value delta, which is 
//defined as the difference in velocity between the two motors.


#include "Motor.h"
#include "TimeIntegral.h"

#define debug true //do we want to print to the serial monitor?

//pinouts
#define rightSensorPin A0
#define leftSensorPin A2
#define switchPin A1

#define left 1
#define right -1
//create custom motor objects for both motors
//Motor(pin, reversePolarity?)
Motor leftMotor(3,true);
Motor rightMotor(1,true);


int defaultVel = 255;//average motor velocity when not turning
int turnAroundVel = -10;//average motor velocity during turn around sequence
int maxDelta = 510;//max value for the absolute value of the output of the controller

int maxIntegrationTime = 800;//How long in ms do we want it to take for our integrator to saturate. This value is used to tune the gain of the integrator. 
int timeStep = 10;//milliseconds between each loop (used to throttle refresh rate)
float kI = float(maxDelta)/float(maxIntegrationTime);//gain of the integrator
float kP = 50;//gain of the proportional term
int sideWeAreOn = 0;//maps to left (1), right(-1) or on the edge (0);
float onLinePenaltyFactor = 7;//cost of both sensors being on line
float offLinePenaltyFactor = 2.5;//cost of neither sensor being on line
TimeIntegral integralObj(maxIntegrationTime/timeStep);//this object is used for leaky integration. parameter is how many values it should store.


int irSenseThreshold = 500;//value greater than this means we are over line

//how often do we want to print while we are in debug mode?
const int serialRefreshTime = 1000;//ms
//when do we print next?
unsigned long int nextPrintTime = 0;//ms
//what is the time that the current loop started?
unsigned long int thisLoopTime = 0;
//what time did the last loop start?
unsigned long int lastLoopTime = 0;

//did we already print this frame?
bool printThisFrame;

void setVel(int vel)
{
  //sets average velocity of robot
  rightMotor.setVel(vel);
  leftMotor.setVel(vel);
}

void setDelta(int delta)
{
  //sets difference in velocity between the motors
  rightMotor.setDelta(delta/2);
  leftMotor.setDelta(-delta/2);
}

bool readSensor(int direction)
{
  //reads left or right sensor, as defined by direction
  int reading;

  //get appropriate reading
  if (direction == right)
  {
    reading = analogRead(rightSensorPin);
  } else
  {
    reading = analogRead(leftSensorPin);
  }
  
  printValue(reading);//debug

  //is the reading above threshold value?
  return reading> irSenseThreshold;
}

void setup()
{
  Serial.begin(9600);//setup serial port
  
  //setup pins
  pinMode(rightSensorPin, INPUT_PULLUP);
  pinMode(leftSensorPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
}

void loop()
{
  thisLoopTime = millis();//keep track of when this loop started
  int dt = thisLoopTime - lastLoopTime;//dt is the time between this loop start and last loop start
  lastLoopTime = thisLoopTime;//we don't need lastLoopTime anymore. store current loop time there (for next loop)
  printThisFrame = false;//reset print
  
  bool rightOn = readSensor(right);//is the right sensor on the line?
  bool leftOn = readSensor(left);//is the left sensor on the line?
  
  int error = 0;//error used for the controller
  
  if(rightOn^leftOn)//are we on an edge?
  {
    integralObj.reset();//reset the integrator

    //which edge are we on? keep track of that
    if (rightOn)
    {
      sideWeAreOn = right;
    } else
    {
      sideWeAreOn = left;
    }
  }

  
  if(!rightOn && !leftOn)//are both sensors off the line?
  {
    error = offLinePenaltyFactor*sideWeAreOn;//set error appropriately
  }

  
  if (rightOn && leftOn)//are both sensors on the line?
  {
    error = -onLinePenaltyFactor*sideWeAreOn;//set error appropriately
  }

  
  integralObj.add(error*dt);//add our current error to the integral object

  
  int integralOfError = integralObj.calculate();//integrate error over the object's entire timespan

  
  integralOfError = saturate(integralOfError,maxIntegrationTime);//saturate integrator

  printValue(integralOfError);//used for debugging

  
  int delta = kP*error + int(kI*float(integralOfError));// this is our controller
  
  delta = saturate(delta,maxDelta);//saturate delta
  
  if (!digitalRead(switchPin))//has the switch been pressed?
  {
    //if so, turn around
    turnAround(sign(delta));
  } else
  {
    //otherwise, send controller output to the motors
    setVel(defaultVel - abs(delta)/2);//makes sure one motor always moves at the default velocity (this makes sure the delta doesn't saturate on one motor)
    setDelta(delta);//set difference in velocity between motors
  }
  
  printValue(delta);//used for debugging
  
  
  //if the time since last loop is less than our refresh period, wait until refresh period has been reached.
  if (dt<timeStep)
  {
    delay(timeStep-dt);
  }
}

void printValue(String value)
{
  //if we already printed this loop, keep printing
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
  nextPrintTime = thisLoopTime+serialRefreshTime;//keep track of next time we will print
  printThisFrame = true;//keep track of the fact that we printed this loop already
}
void printValue(int value)
{
  //overload for ints
  printValue(String(value));
}

int saturate(int  var, int maxValue)
{
  //clamps var to [-abs(maxvalue),abs(maxValue)]
  if (abs(var)>maxValue)
  {
    var = sign(var)*maxValue;
  }
  return var;
}
int sign(int var)
{
  //returns 1 for positive, 0 for 0 and -1 for negative
  if (var == 0)
  {
    return 0;
  }
  if (var<0)
  {
    return -1;
  }
  return 1;
}

void turnAround(int direction)
{
  //turn around sequence
  //turns in the direction specified by direction
  setVel(turnAroundVel);//sets average velocity of robot. we want it to back up slightly while turning
  if (direction==0)//if direction was set to zero, pick a side and turn that way
  {
    direction = sideWeAreOn;
  }
  //turn in the direction specified
  setDelta(direction*(255-abs(turnAroundVel))*2);
  
  delay(800);//wait until we have probably turned far enough
}
