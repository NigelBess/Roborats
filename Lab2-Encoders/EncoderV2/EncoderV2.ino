using namespace std;
#include <AFMotor.h> 
#include <SoftwareSerial.h>

#define forward 1
#define backward 2

#define interruptPin 2      //Only pins 2 and 3 are configured for hardware interrupts on the Arduino board
#define encoderPin 2        // break-beam sensor connected to this pin (pin 2, must be pin 2 or 3 since they have hardware interrupts 0 and 1, respectively)

volatile int ticks = 0;     //varibles changed by an interrupt must be declared as volatile
int byteSpeedTable[][2] = {{49, 100},{ 50, 178},{ 51, 255}};
int dataLogTime = 200;//ms

AF_DCMotor motor(2, MOTOR34_1KHZ);   // create motor on  M3, 1KHz pwm

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);           //Declares the interrupt pin as an input source
  attachInterrupt(0,countTick, FALLING);  // runs countTick when voltage changes from 5 to 0
  go(forward);
  setSpeed(byteSpeedTable[0][1]);
}

void loop() {
  logData();
  if (Serial.available()>0) {
     setSpeed(serialToSpeed(Serial.read()));  // read the incoming byte:
  }
  delay(dataLogTime);
}
void logData(){
  Serial.print(ticks);   //Every 1/2 second, the number of interrupts that have been registered is printed
  Serial.print(",");
  Serial.println(int(millis())/1000.0);
}
void setSpeed(int speed){
  motor.setSpeed(speed);
  go(forward);
  printSpeed(speed);
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

int serialToSpeed(int serial){
  for(int i = 0; i< sizeof(byteSpeedTable)/sizeof(byteSpeedTable[0]);i++){
    if (byteSpeedTable[i][0] == serial){
      return byteSpeedTable[i][1];
    }
  }
  return 0;
}
