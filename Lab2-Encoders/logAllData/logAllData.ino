using namespace std;
#include <AFMotor.h> 
#include <SoftwareSerial.h>

#define forward 1
#define backward 2

#define interruptPin 2      //Only pins 2 and 3 are configured for hardware interrupts on the Arduino board
#define encoderPin 2        // break-beam sensor connected to this pin (pin 2, must be pin 2 or 3 since they have hardware interrupts 0 and 1, respectively)

volatile int ticks = 0;     //varibles changed by an interrupt must be declared as volatile
int desiredSpeed[] = {100, 178,255};
String outputFileNames[] = {"slow","medium","fast"};
int dataLogTime = 100;//ms
int numDataPoints = 100;

AF_DCMotor motor(2, MOTOR34_1KHZ);   // create motor on  M3, 1KHz pwm

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);           //Declares the interrupt pin as an input source
  attachInterrupt(0,countTick, FALLING);  // runs countTick when voltage changes from 5 to 0
  logTrialInfo();
  for (int k = 0; k<3;k++)
  {
    for (int i = 0;i<sizeof(desiredSpeed)/sizeof(desiredSpeed[0]);i++){
      setSpeed(desiredSpeed[i]);
      for (int j = 0;j<numDataPoints;j++)
      {
        logData();
        delay(dataLogTime);
      }
    }
  }
  setSpeed(0);
}

void loop() {
  
}
void logTrialInfo()
{
  Serial.print("Motor Speeds: ");
  for (int i = 0;i<sizeof(desiredSpeed)/sizeof(desiredSpeed[0]);i++){
    Serial.print(desiredSpeed[i]);
    Serial.print(" ");
  }
  Serial.print(". ");
  Serial.print(numDataPoints);
  Serial.println(" data points per trial.");
}
void logData(){
  Serial.print(ticks);   //Every 1/2 second, the number of interrupts that have been registered is printed
  Serial.print(",");
  Serial.println(millis());
}
void setSpeed(int speed){
  motor.setSpeed(speed);
  go(forward);
}
void printSpeed(int speed){
  Serial.print("SETTING MOTOR SPEED TO ");
  Serial.println(speed);
}
void go( int direction) {      //Drives the encoder wheel
  motor.run(direction);
}
void countTick() {         //This Function is called by a change from HIGH to LOW on pin 3
  ++ticks;                 //Increment the ticks variable every time the interrupt is called
}

