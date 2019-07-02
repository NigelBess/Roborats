using namespace std;
#include <AFMotor.h> 
#include <SoftwareSerial.h>

//constants for legibility
#define forward 1
#define backward 2
#define left -1
#define right 1

//pins for the encoders
#define rightEncoderPin 2
#define leftEncoderPin 3
#define lcdPin 13

//create a serial display
SoftwareSerial lcd =  SoftwareSerial(lcdPin, lcdPin);  //Change Tx and Rx Pins to pins of our choosing

const int buttonPin = 9;     // the number of the pushbutton pin
int targetEncoderValue = 0; //during loop, this program moves the robot to match the target encoder value

//encoder readings (changed by interrupt)
volatile int rightEncoderValue = 0;     
volatile int leftEncoderValue = 0;     

//motor speeds to cycle through when you press a button
int speeds[] = {178,215,255};
int speedIndex = 0;//index of current speed

//target distances to cycle through
int distances[] = {4,7};//feet
int distanceIndex = -1;//index of current target distance
//distanceIndex starts at -1 so that the first target distance is at index 0


const int distanceCalibrationConstant = 258; //ticks per foot of robot travel

//how long the motor pauses after reaching target distance
const int pauseTime = 1000;//ms

//used to decrease LCD print rate
const int screenRefreshTime = 1000;//ms
int lastPrintTime = 0;

//initialize motors
AF_DCMotor leftMotor(3, MOTOR34_1KHZ);   // create motor on  M3, 1KHz pwm
AF_DCMotor rightMotor(4, MOTOR34_1KHZ);   // create motor on  M4, 1KHz pwm

AF_DCMotor *currentMotor;//used for changing the speed of a single motor at a a time

void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);//setup button pin
  pinMode(lcdPin, OUTPUT);//setup lcd pin

  //setup encoder pins and attach appropriate interrupts
  pinMode(rightEncoderPin, INPUT_PULLUP); 
  pinMode(leftEncoderPin, INPUT_PULLUP);
  attachInterrupt(rightEncoderPin-2,rightTick, FALLING);
  attachInterrupt(leftEncoderPin-2,leftTick, FALLING);
  //[pin number]-2 converts from d2 and d3 to the corresponding 1 and 2

  //initialize lcd
  lcd.begin(9600);
  lcd.print("?f"); 

  //initialize speeds, and start moving towards the first target
  setSpeed(speeds[0]);
  goToNextDistance();
}

void loop() 
{
  //have we reached the goal? If so stop for a moment, then move to the next target
  if(targetEncoderValue<max(rightEncoderValue,leftEncoderValue))
  {
    pause();//stop the robot for the time defined by pauseTime
    goToNextDistance();//start moving to the next target distance
  }

  //is the button being pressed? if so, cycle the speed
  if(!digitalRead(buttonPin))
  {
    cycleSpeed();
  }

  //print encoder values to the lcd
  printEncoders();
}


void setSpeed(int speed)
{
  //sets the speed of both motors
  leftMotor.setSpeed(speed);
  rightMotor.setSpeed(speed);
}
void setSpeed(int speed, int wheel)
{
  //overload that sets the speed of one motor at a time
  getMotor(wheel);//set currentMotor to the appropriate motor
  (*currentMotor).setSpeed(speed);//set its speed
}
void cycleSpeed()
{
  //increment the index, and change it to the appropriate index if it's out of bounds
  speedIndex++;
  speedIndex = arrayIndex(speedIndex,sizeof(speeds)/sizeof(speeds[0]));

  
  setSpeed(speeds[speedIndex]);
}
void goToNextDistance()
{
  resetEncoders();
  
  //increment the index, and change it to the appropriate index if it's out of bounds
  distanceIndex++;
  distanceIndex = arrayIndex(distanceIndex,sizeof(distances)/sizeof(distances[0]));

  //set the new target encoder value (converting feet to encoder counts)
  targetEncoderValue = distances[distanceIndex]*distanceCalibrationConstant;

  //start moving towards new target
  go(forward);
}

void printEncoders()
{
  if ((millis()-lastPrintTime)< screenRefreshTime)
  {
    return;
  }
  lcd.print("?f"); //clear screen

  //display encoder readings, separated by a comma
  lcd.print(leftEncoderValue);
  lcd.print(",");
  lcd.print(rightEncoderValue);
  lcd.print("  ");
  
  lastPrintTime = millis();//keep track of the last time it printed
}

void pause(int time)
{
  //stops the motors for a set amount of time
  stopMotors();
  delay(time);
}

void pause()
{
  //overload for not declaring the time to pause
  pause(pauseTime);
}

void stopMotors()
{
  //stops both motors
  go(RELEASE);
}
void resetEncoders()
{
  //resets both encoder counts to zero
  rightEncoderValue = 0;
  leftEncoderValue = 0;
}
void go(int direction)
{
  //direction is expected to be forward or backward (defined in #define statements)
  //moves both motors in said direction
  leftMotor.run(direction);
  rightMotor.run(direction);
}
void go(int direction, int wheel) 
{ 
  //overload that sets the rotation direction of one motor at a time
   getMotor(wheel);//set currentMotor to the appropriate motor
  (*currentMotor).run(direction);//turn that motor the correct direction
}
void getMotor(int wheel)
{
  //sets current motor based on the chosen wheel
  //wheel is expected to be left or right (defined in #define statements)
  switch (wheel) 
  {
    case left:
      currentMotor = &leftMotor;
      break;
    case right:
      currentMotor = &rightMotor;
      break;
  }
    
}
void rightTick() 
{ 
  //interrupt function for right encoder
  //increments right encoder Count
  ++rightEncoderValue;
}

void leftTick()
{ 
  //interrupt function for left encoder
  //increments left encoder Count
  ++leftEncoderValue;
}
int arrayIndex(int index,int arraySize)
{
  //used for cycling through an array
  //gets the appropriate index when the index exceeds array dimensions
  return index%arraySize;
}
