#define encoderPin 2

int count  = 0;
int lastCount = -1;
void setup()
{
  Serial.begin(9600);
  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPin),countTick, FALLING); 
}
void loop()
{
  if (count != lastCount)
  {
    Serial.println(count);
  }
  lastCount = count;
}
void countTick()
{
  count++;
}
