
#include "Motor.h"
#define rangeSensorPin A1
#define debug true
#include "math.h"
#define intMult 1000

Motor leftMotor(4,true);
Motor rightMotor(3,true);

int calibrationData[][2] = {{0,40},{27,20},{58,15},{139,10},{396,5},{455,4}};

int defaultVel = 200;//out of 255
int k = 30;//constant for proportional control
float targetDistance = 20.0;

bool printThisFrame = false;

const int serialRefreshTime = 1000;//ms
unsigned long int lastPrintTime = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(rangeSensorPin, INPUT); 
  resetVel();
}

void loop()
{
  printThisFrame = false;
  
  float distance = getDistance();
  printValue(analogRead(rangeSensorPin));
  printValue(distance);
  setDelta((targetDistance-distance)*k);
}

void setVel(int vel)
{
  rightMotor.setVel(vel);
  leftMotor.setVel(vel);
}
void resetVel()
{
  setVel(defaultVel);
  setDelta(0);
}

void setDelta(float delta)
{
  rightMotor.setDelta(delta/2);
  leftMotor.setDelta(-delta/2);
}
float getDistance()
{
  return interpolateFromCalibration(analogRead(rangeSensorPin));
}

void printValue(float value)
{
  if(!printThisFrame)
  {
    if (!debug || ((millis()-lastPrintTime)< serialRefreshTime))
    {
      return;
    }
    Serial.println("");
  }
  Serial.println(value); 
  lastPrintTime = millis();//keep track of the last time it printed
  printThisFrame = true;
}
float interpolateFromCalibration(int reading)
  {
    int n = sizeof(calibrationData)/sizeof(calibrationData[0]);
    for (int i = 0;i<n;i++)
    {
      if (calibrationData[i][0]>=reading)
      {
        if (i==0) return float(calibrationData[0][1]); 
        float m = float((calibrationData[i][1]-calibrationData[i-1][1]))/float(calibrationData[i][0]-calibrationData[i-1][0]);
        float b = float(calibrationData[i][1])-float(calibrationData[i][0])*m;
        return (m*float(reading)+b);
      }
    }
    return float(calibrationData[n][1]);
  }
