
#include "Motor.h"
#include "math.h"

#define debug true //do we want to print to the serial monitor?

//pinouts
#define photoResPin A2
#define rangeSensorPin A1

//create custom motor objects for both motors
//Motor(pin, reversePolarity?)
Motor leftMotor(4,true);
Motor rightMotor(3,true);

//imperical data of the form {voltage reading (int), distance (cm)} 
//used to get distance from sensor reading
//int calibrationData[][2] = {{0,40},{27,20},{58,15},{139,10},{396,5},{455,4}};short range sensor data
int calibrationData[][2] = {{94,70},{110,60},{126,50},{149,40},{188,30},{268,20},{305,17},{341,15},{414,12},{487,10},{638,7}};
//velocity when moving forward
int defaultVel = 200;//out of 255

//velocity when moving backward (out of corner)
int backUpVel = -200;

//this script uses proportional control to maintain distance to the wall
//the controller was discretized into two regions, one for when the robot is too close,
//and one for when it is too far. The two gains for the proportional controller are defined below
int farGain = 40;
int closeGain = 25;

//wall follower target distance to wall (cm)
float targetDistance = 25.0;

//if distance drops below this number, assume we have hit a corner
float frontBumpDistance = 7;

//maxDelta saturates the controller. It defines the max variation in speed that the controller can cause
int maxDelta = 255;

//if the reading from the photoresistor drops below this value, start the wall following sequence
int photoResThreshold = 100;

//used to make sure we don't print too much in debug mode
bool printThisFrame = false;

//state machine variable (two states: in corner or not in corner)
bool inCorner = false;

//how often do we want to print while we are in debug mode?
const int serialRefreshTime = 1000;//ms
unsigned long int lastPrintTime = 0;//keeps track of the last time we printed

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
  pinMode(rangeSensorPin, INPUT);
  pinMode(photoResPin, INPUT_PULLUP);

  //wait until we sense the light
//  while(analogRead(photoResPin)>photoResThreshold)
//  {
//  }

   //start moving forward at default velocity without turning
  resetVel();
}

void loop()
{
  //print() checks if we want to print this frame, and will set this var to true if we do
  printThisFrame = false;
  
  int k;//gain of controller
  
  float distance = getDistance();//how far are we from the wall

  
  if (inCorner)
  {
    if (distance>targetDistance) //check if we have left the corner
    {
      //if we left the corner, keep track of that and start moving forward again
      inCorner = false;
      setVel(defaultVel);
    }
  } else
  {
    if (distance<frontBumpDistance) //check if we are in a corner
    {
      //if we have hit a corner, keep track of that and start backing up
      inCorner = true;
      setVel(backUpVel);
    }
  }

  
  int error = (targetDistance-distance);//error used for proportional control

  //discretize the controller
  if (error > 0)//error>0 means we are too far from the wall
  {
    k = farGain;
  } else
  {
    k = closeGain;
  }

  //sets appropriate change in velocities of motors
  setDelta(error*k);

  //print for debugging and sensor calibration
  printValue(analogRead(rangeSensorPin));
  printValue(distance);
  
}


void resetVel()
{
  //makes the motors move forward at default velocity
  setVel(defaultVel);
  setDelta(0);
}

void setDelta(float delta)
{
  //delta is the variation in speed between motors (added/subtracted to their nominal speed)

  //saturate delta value
  if(abs(delta)>maxDelta)
  {
    delta *= maxDelta/abs(delta);
  }

  //distribute the speed delta evenly accross the two motors
  rightMotor.setDelta(delta/2);
  leftMotor.setDelta(-delta/2);
}

float getDistance()
{
  //uses calibration table to interpolate from the analog signal
  return interpolateFromCalibration(analogRead(rangeSensorPin));
}

void printValue(float value)
{
  //check if we have not already printed this loop
  if(!printThisFrame)
  {
    //is it time to print again? if not return
    if (!debug || ((millis()-lastPrintTime)< serialRefreshTime))
    {
      return;
    }

    //if we haven't already printed this loop, start a new line
    Serial.println("");
  }

  //if we made it this far, it's time to print
  //
  Serial.println(value); 
  lastPrintTime = millis();//keep track of the last time it printed
  printThisFrame = true;//if we printed this loop, keep printing this loop
}

float interpolateFromCalibration(int reading)
  {
    //Interpolate analog reading to a distance, referencing calibration data table

    //n = number of data points in table
    int n = sizeof(calibrationData)/sizeof(calibrationData[0]);
    for (int i = 0;i<n;i++)
    {
      //calibration table is sorted in ascending order by reading (column 0)

      //if the reading value of the next data point is higher than the current reading
      //then we have found the data points to interpolate between
      if (calibrationData[i][0]>=reading)
      {
        if (i==0) return float(calibrationData[0][1]); 
        //solve for m and b in line defined by y = mx+b (y = distance, x = reading)
        float m = float((calibrationData[i][1]-calibrationData[i-1][1]))/float(calibrationData[i][0]-calibrationData[i-1][0]);
        float b = float(calibrationData[i][1])-float(calibrationData[i][0])*m;

        //solve for y (distance)
        return (m*float(reading)+b);
      }
    }
    //if we didn't find two values to interpolate between, just return the max value on the table
    return float(calibrationData[n][1]);
  }
