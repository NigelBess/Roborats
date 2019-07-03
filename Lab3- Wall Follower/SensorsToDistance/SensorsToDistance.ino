#define inputP2YAO21 A1

//used to decrease Serial print rate
const int serialRefreshTime = 1000;//ms
unsigned long int lastPrintTime = 0;
float calibrationConstants[] = {-.1062,41.9419};

void setup() 
{
  pinMode(inputP2YAO21, INPUT); 
  Serial.begin(9600);
}

void loop() 
{
  printValue(analogVoltageToDistance(analogRead(inputP2YAO21),calibrationConstants));
}
void printValue(float value)
{
  if ((millis()-lastPrintTime)< serialRefreshTime)
  {
    return;
  }

  Serial.println(value); 
  lastPrintTime = millis();//keep track of the last time it printed
}
float analogVoltageToDistance(int signal, float *constants)
{
  return float(signal)*constants[0]+constants[1];
}
