#include <AFMotor.h>
#include <SoftwareSerial.h>

#define backward 1
#define forward 2
#define left -1
#define right 1

#define debugMode false//true makes debug statements print to LCD


AF_DCMotor rightMotor(3,MOTOR34_1KHZ);
AF_DCMotor leftMotor(4,MOTOR34_1KHZ);

//for turning, one motor goes forward and one goes backward
AF_DCMotor *forwardTurnMotor;//pointer to the motor that we want to go forward
AF_DCMotor *backwardTurnMotor;//pointer to the motor that we want to go forward
const int msPerDegree = 20;
const int sideStepPauseTime = 50;

int mainSpeed = 200;//out of 255
//fastspeed and slowspeed are used on opposite wheels in order to turn while moving in one direction
int fastSpeed = 255;
int slowSpeed = 50;

//const int msPerDegree = 20;//ms/degree - conversion factor between time spent turning and degrees turned

const float driftCorrectionFactor = 1;//how much faster should the right motor turn

void setup() 
{   
   resetSpeed();//resets speed of motors
}

void loop() 
{

}
void go(int direction)
{
  //moves forward or backward
  //define statements for forward and backward match desired direction
  rightMotor.run(direction);
  leftMotor.run(direction);
}
void halt()
{
  //stops motor
  rightMotor.run(RELEASE);
  leftMotor.run(RELEASE);
}
void stepBack(int time)
{
  //moves robot backwards for set time
  go(backward);
  delay(time);
  halt();//stops motors
}
void stepBack(int direction, int time)
{
  //steps backwards while turning in specified direction (left or right, as defined in define statements)
  setDirection(direction);//sets forward and backward motor
  //convert from pointer into desired motor, and set speed appropriately
  (*forwardTurnMotor).setSpeed(slowSpeed);//because we are backing up, forward motor just turns slower backwards
  (*backwardTurnMotor).setSpeed(fastSpeed);//go faster with the backward motor
  stepBack(time);//after setting speed, step back while moving motors with different speeds
  resetSpeed();//reset motor speeds
}


void turn(int direction, int degrees)
{
  //turns robot in place in specified direction by specified degrees
  setDirection(direction);//sets forward and backward motor
  int turnTime = degrees*msPerDegree;//convert from degrees to time, using conversion factor
     halt();//stop motors
     //run forward and backward motors in appropriate direction
     (*forwardTurnMotor).run(forward);
    (*backwardTurnMotor).run(backward);
    delay(turnTime);//turn long enough to match desired degrees
    halt();//stop the motors after that amount of time
}
void turn(int signedDegrees)
{
  //overload that allows you to specify turn with signed degrees instead of degrees and direcion
  turn(2*signedDegrees>0-1,abs(signedDegrees));//first param converts signedDegrees into direction (1 or -1).
  //second param is the amount we want to turn
}
void setDirection(int direction)
{
  //sets forward and backward motor, dependent on the direction we want to turn
  if (direction==right)
  {
    // if we want to turn right, the forward motor will be the left and the backward motor will be the right
    forwardTurnMotor = &leftMotor;
    backwardTurnMotor = &rightMotor;
  } else
  {
    // if we want to turn left, the forward motor will be the right and the backward motor will be the left
   forwardTurnMotor = &rightMotor;
   backwardTurnMotor = &leftMotor;
  }
}
void resetSpeed()
{
  //resets the speed for plain forward or backward motion
  //correction factor accounts for drift of motor (imperically determined)
  rightMotor.setSpeed(min(mainSpeed*driftCorrectionFactor,255));//min with 255 to make sure we get a reasonable 1 byte value
  leftMotor.setSpeed(mainSpeed);
}
void sideStep(int direction, int time)
{
  //steps backward and translates robot to one side defined by direction (-1 or 1) corresponding to left or right
  pause(sideStepPauseTime);//stop robot to negate inertial effects
  //first step back and turn in one direction, then the other. this translates the robot back and to one side
  stepBack(-direction,time);
  pause(sideStepPauseTime);
  stepBack(direction,time);
}
void pause(int time)
{
  //stops the motors for a set amount of time
  halt();
  delay(time);
}

