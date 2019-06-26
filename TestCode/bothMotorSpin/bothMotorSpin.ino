#include <AFMotor.h>

AF_DCMotor rightMotor(3,MOTOR34_1KHZ);
AF_DCMotor leftMotor(4,MOTOR34_1KHZ);

int motorSpeed = 200;//out of 255
int pauseTime = 1000;//ms

void setup() 
{
  rightMotor.setSpeed(motorSpeed);
  leftMotor.setSpeed(motorSpeed);
}

void loop() 
{
  rightMotor.run(FORWARD);
  leftMotor.run(FORWARD);
  delay(pauseTime);
  leftMotor.run(BACKWARD);
  rightMotor.run(BACKWARD);
  delay(pauseTime);
}
