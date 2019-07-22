#include "ServoObj.h"
#define servoPin 9

ServoObj servo(servoPin);

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  if(!(Serial.available()>0))
  {
    return;
  }
  int incomingByte = Serial.read();
  int pos = 0;
  switch (incomingByte)
  {
    case 49: //1
    pos = 20;
    break;
    case 50: //2
    pos = 90;
    break;
    case 51: //3
    pos = 170;
    break;
  }
  setPos(pos);
}
void setPos(int pos)
{
  Serial.println("setting position to: " + String(pos));
  servo.setPos(pos);
  //servo2.write(pos);
}

