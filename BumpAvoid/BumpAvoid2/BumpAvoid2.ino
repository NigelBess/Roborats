void setup() {
  // put your setup code here, to run once:

}

void loop() {#include <AFMotor.h>

#define backward 1
#define forward 2
#define left -1
#define right 1

AF_DCMotor rightMotor(3,MOTOR34_1KHZ);
AF_DCMotor leftMotor(4,MOTOR34_1KHZ);
AF_DCMotor *forwardTurnMotor;
AF_DCMotor *backwardTurnMotor;

int mainSpeed = 150;//out of 255
int fastSpeed = 255;
int slowSpeed = 50;
const int rightLimitSwitchPin = 0;
const int leftLimitSwitchPin = 1;
int stepBackTime = 500;//ms
bool rightSwitchTripped;
bool leftSwitchTripped;
int turnTime = 100000;//ms

void setup() 
{
  pinMode(rightLimitSwitchPin, INPUT_PULLUP);
   pinMode(leftLimitSwitchPin, INPUT_PULLUP);
   
   resetSpeed();
   go(forward);
}

void loop() 
{
  
  rightSwitchTripped = !digitalRead(rightLimitSwitchPin);
  leftSwitchTripped = !digitalRead(leftLimitSwitchPin);
  if(rightSwitchTripped || leftSwitchTripped)
  {
    int direction = 2*rightSwitchTripped-1;
     sideStep(direction);
    go(forward);
  }

}
void go(int direction)
{
  
  rightMotor.run(direction);
  leftMotor.run(direction);
}
void halt()
{
  rightMotor.run(RELEASE);
  leftMotor.run(RELEASE);
}
void stepBack()
{
  go(backward);
  delay(stepBackTime);
  halt();
}
void stepBack(int direction)
{
  setDirection(direction);
  (*forwardTurnMotor).setSpeed(slowSpeed);
  (*backwardTurnMotor).setSpeed(fastSpeed);
  stepBack();
  resetSpeed();
}

void turn(int direction)
{
  halt();

  (*forwardTurnMotor).run(forward);
  (*backwardTurnMotor).run(backward);
  delay(turnTime);
  halt();
}
void setDirection(int direction)
{
  if (direction==right)
  {
    forwardTurnMotor = &leftMotor;
    backwardTurnMotor = &rightMotor;
  } else
  {
   forwardTurnMotor = &rightMotor;
   backwardTurnMotor = &leftMotor;
  }
}
void resetSpeed()
{
  rightMotor.setSpeed(mainSpeed);
   leftMotor.setSpeed(mainSpeed);
}
void sideStep(int direction)
{
  stepBack(-direction);
  stepBack(direction);
}
  // put your main code here, to run repeatedly:

}
