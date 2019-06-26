#include <SoftwareSerial.h>

#define rxPin 13  
#define txPin 13
SoftwareSerial lcd =  SoftwareSerial(rxPin, txPin);
void setup()
{

  pinMode(txPin, OUTPUT);
  lcd.begin(9600);
  clear();
  lcd.print("hello world");
}
void loop()
{
}
void clear()
{
  lcd.print("?f");
  resetLCDCursor();  
}
void resetLCDCursor()
{
  lcd.print("?x00?y0");
}
