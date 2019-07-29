#include <Servo.h>
#define servoPin 9
#define servoPin2 10

Servo servo;
Servo servo2;

void setup()
{
  servo.attach(servoPin);
  servo2.attach(servoPin2);
  Serial.begin(9600);
}
void loop()
{
  if(!(Serial.available()>0))
  {
    return;
  }
  int pos = Serial.parseInt();
  setPos(pos);
}
void setPos(int pos)
{
  Serial.println("setting position to: " + String(pos));
  //servo.write(pos);
  servo2.write(pos);
}

