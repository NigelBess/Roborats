#include <AFMotor.h>

AF_DCMotor rightMotor(1,MOTOR12_1KHZ);
AF_DCMotor leftMotor(3,MOTOR34_1KHZ);

int motorSpeed = 255;//out of 255
int pauseTime = 1000;//ms

void setup() 
{
  rightMotor.setSpeed(motorSpeed);
  leftMotor.setSpeed(motorSpeed);
  rightMotor.run(FORWARD);
  leftMotor.run(FORWARD);
}

void loop() 
{
//  rightMotor.run(FORWARD);
//  leftMotor.run(FORWARD);
//  delay(pauseTime);
//  leftMotor.run(BACKWARD);
//  rightMotor.run(BACKWARD);
//  delay(pauseTime);
}
