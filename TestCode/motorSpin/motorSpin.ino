#include <AFMotor.h>

AF_DCMotor rightMotor(3,MOTOR34_1KHZ);

int motorSpeed = 200;//out of 255
int pauseTime = 3000;//ms

void setup() 
{
  rightMotor.setSpeed(motorSpeed);
}

void loop() 
{
  rightMotor.run(FORWARD);
  delay(pauseTime);
  rightMotor.run(BACKWARD);
  delay(pauseTime);
}
