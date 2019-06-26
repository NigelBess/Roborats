#include <AFMotor.h>
#include <SoftwareSerial.h>

#define backward 1
#define forward 2
#define left -1
#define right 1
#define debugMode true


#define rxPin 13  
#define txPin 13
SoftwareSerial lcd =  SoftwareSerial(rxPin, txPin);

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
int turnTime = 100;//ms
const int msPerDegree = 60;//ms/degree
int rotation = 0;
int lastTurnTime;
const int turnTimeThreshold = 2000;
const int keepTryTime = 1000;

void setup() 
{
  pinMode(rightLimitSwitchPin, INPUT_PULLUP);
   pinMode(leftLimitSwitchPin, INPUT_PULLUP);
    pinMode(txPin, OUTPUT);
  lcd.begin(9600);
  clear();
   lastTurnTime = millis();
   rotation = 0;
   resetSpeed();
   go(forward);
   debug(rotation);
}

void loop() 
{
   if(((millis()-lastTurnTime)>turnTimeThreshold) && abs(rotation)>0)
  {
    debug(999);
    turn(-rotation);
    rotation = 0;
    go(forward);
  }
  rightSwitchTripped = !digitalRead(rightLimitSwitchPin);
  leftSwitchTripped = !digitalRead(leftLimitSwitchPin);
  if(rightSwitchTripped || leftSwitchTripped)
  {
    
    int direction = 2*rightSwitchTripped-1;
    stepBack();
    turn(-direction,10);//turn 10 degree
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


void turn(int direction, int degrees)
{
  setDirection(direction);
  int turnTime = degrees*msPerDegree;
     halt();
     (*forwardTurnMotor).run(forward);
    (*backwardTurnMotor).run(backward);
    delay(turnTime);
    halt();
    rotation+=direction*degrees;
    lastTurnTime = millis();
    debug(rotation);
}
void turn(int signedDegrees)
{
  turn(2*signedDegrees>0-1,abs(signedDegrees));
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
void clear()
{
  lcd.print("?f");
  lcd.print("?x00?y0");
}

void debug(int message)
{
  if(debugMode)
  {
    clear();
    lcd.print(message);
  }
}

