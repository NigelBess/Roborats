#define inputPin A0

//used to decrease Serial print rate
const int serialRefreshTime = 100;//ms
int lastPrintTime = 0;

#include "Motor.h"

Motor motor(3);

void setup() 
{
  pinMode(inputPin, INPUT_PULLUP);
  Serial.begin(115200);
  motor.setVel(255);
}

void loop() 
{
    Serial.println(analogRead(inputPin));
}

