#define inputPin A4

//used to decrease Serial print rate
const int serialRefreshTime = 100;//ms
int lastPrintTime = 0;

void setup() 
{
  pinMode(inputPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() 
{
  printValue(analogRead(inputPin));
}
void printValue(int value)
{
  if ((millis()-lastPrintTime)< serialRefreshTime)
  {
    return;
  }

  Serial.println(value);
  
  lastPrintTime = millis();//keep track of the last time it printed
}

