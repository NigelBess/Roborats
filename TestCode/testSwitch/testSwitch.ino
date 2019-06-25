#include <AFMotor.h>

AF_DCMotor rightMotor(3,MOTOR34_1KHZ);

int motorSpeed = 200;//out of 255
const int limitSwitchPin = 0;

void setup() 
{
  pinMode(limitSwitchPin, INPUT_PULLUP);
  rightMotor.setSpeed(motorSpeed);
}

void loop() 
{
  if(!digitalRead(limitSwitchPin))
  {
    rightMotor.run(FORWARD);
  } else
  {
    rightMotor.run(RELEASE);
  }
}
