#include <AFMotor.h> 
#include <SoftwareSerial.h> 
#define interruptPin 2      //Only pins 2 and 3 are configured for hardware interrupts on the Arduino board
#define encoderPin 2        // break-beam sensor connected to this pin (pin 2, must be pin 2 or 3 since they have hardware interrupts 0 and 1, respectively)
int incomingByte = 0;   // for incoming serial data
int motorSpeed = 0;
double myTime = 0;
double myMilli=0;
volatile int ticks = 0;     //varibles changed by an interrupt must be declared as volatile
AF_DCMotor motor(2, MOTOR34_1KHZ);   // create motor on  M3, 1KHz pwm

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT);           //Declares the interrupt pin as an input source
  digitalWrite(interruptPin, HIGH);       // pull up resistor on interrupt pin
  attachInterrupt(0,countTick, FALLING);  // runs countTick when voltage changes from 5 to 0
}

void loop() {
  driveForward();          //This function will drive the encoder wheel forward
  Serial.print(ticks);   //Every 1/2 second, the number of interrupts that have been registered is printed
  Serial.print(",");
  myMilli = int(millis());
  myTime = myMilli/1000;
  Serial.println(myTime);
  delay(500);
  if (Serial.available() > 0) {
     incomingByte = Serial.read();  // read the incoming byte:
     if (incomingByte == 49) {
        Serial.print("SETTING MOTOR TO ");
        Serial.println("LOW");
        motorSpeed = 100;
     }
     if (incomingByte == 50) {
        Serial.print("SETTING MOTOR TO ");
        Serial.println("MEDIUM");
        motorSpeed = 178;
     }
     if (incomingByte == 51) {
        Serial.print("SETTING MOTOR TO ");
        Serial.println("HIGH");
        motorSpeed = 255;
     }
  }
}

void driveForward() {      //Drives the encoder wheel
  motor.setSpeed(motorSpeed);
  motor.run(FORWARD);
}

void countTick() {         //This Function is called by a change from HIGH to LOW on pin 3
  ++ticks;                 //Increment the ticks variable every time the interrupt is called
}
