#define inputPin A0

//used to decrease Serial print rate
const int serialRefreshTime = 100;//ms
int lastPrintTime = 0;

#include <AF_DCMotor>

AF_DCMotor motor(3,MOTOR34_1KHz);

void setup() 
{
  pinMode(inputPin, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() 
{
    Serial.println(analogRead(inputPin));
}

