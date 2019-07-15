
#include "Motor.h"
#include "math.h"

#define debug false //do we want to print to the serial monitor?

//pinouts
#define photoResPin A2
#define rangeSensorPin A1

//create custom motor objects for both motors
//Motor(pin, reversePolarity?)
Motor leftMotor(1,true);
Motor rightMotor(2
,false);

//imperical data of the form {voltage reading (int), distance (cm)} 
//used to get distance from sensor reading
//close range sensor data: int calibrationData[][2] = {{0,40},{27,20},{58,15},{139,10},{396,5},{455,4}};short range sensor data
int calibrationData[][2] = {{0,200},{94,70},{110,60},{126,50},{149,40},{188,30},{268,20},{305,17},{341,15},{414,12},{487,10},{638,7}};

int maxDelta = 510;//max allowable difference in speed between motors

//this script uses proportional control to maintain distance to the wall
float k = 6;//gain of our p controller

//wall follower target distance to wall (cm)
float targetDistance = 45.0;


//if the reading from the photoresistor drops below this value, start the wall following sequence
int photoResThreshold = 100;

//used to make sure we don't print too much in debug mode
bool printThisFrame = false;


//how often do we want to print while we are in debug mode?
const int serialRefreshTime = 1000;//ms
unsigned long int lastPrintTime = 0;//keeps track of the last time we printed

void setVel(int vel)
{
  //sets average velocity of robot
  //printValue(vel);
  rightMotor.setVel(vel);
  leftMotor.setVel(vel);
}

void setup()
{
  Serial.begin(9600);//setup serial port
  //setup pins
  pinMode(rangeSensorPin, INPUT);
  pinMode(photoResPin, INPUT_PULLUP);

//  wait until we sense the light
  while(analogRead(photoResPin)>photoResThreshold)
  {
  }
}

void loop()
{
  //printValue() checks if we want to print this frame, and will set this var to true if we do
  printThisFrame = false;
  
  
  float distance = getDistance();//how far are we from the wall
  
  float error = (targetDistance-distance);//error used for proportional control

  //delta is the difference in speed between the motors (makes the motors turn)
  float delta =  error*k;

  //saturate delta value
  if (delta>maxDelta) {delta = maxDelta;}
  if (delta<-maxDelta) {delta = -maxDelta;}

  
  //printValue(delta);
  setDelta(delta);

  //print for debugging and sensor calibration
  //printValue(analogRead(rangeSensorPin));
  //printValue(delta);
  printValue(distance);
  
}


void setDelta(float delta)
{
  
  //delta is the variation in speed between motors (added/subtracted to their nominal speed)
  
  setVel(255-abs(int(delta/2)));//this makes sure one motor always turns at max speed
  
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
