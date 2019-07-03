
#include "Motor.h"
#define inputP2YAO21 A1

Motor leftMotor(4);
Motor rightMotor(3);

float calibrationConstants[] = {-.1062,41.9419};

int defaultVel = 200;//out of 255
float k = 100;//constant for proportional control
float targetDistance = 15.0;

void setup()
{
  pinMode(inputP2YAO21, INPUT); 
  resetVel();
}

void loop()
{
  float distance = getDistance();
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
  rightMotor.setDelta(-delta/2);
  leftMotor.setDelta(delta/2);
}
float getDistance()
{
  return analogVoltageToDistance(analogRead(inputP2YAO21),calibrationConstants);
}
float analogVoltageToDistance(int signal, float *constants)
{
  return float(signal)*constants[0]+constants[1];
}

