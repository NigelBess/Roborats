#define inputD120 A0
#define inputYAO21 A1

//used to decrease Serial print rate
const int serialRefreshTime = 1000;//ms
unsigned long int lastPrintTime = 0;

void setup() 
{
  pinMode(inputD120, INPUT);
  pinMode(inputYAO21, INPUT); 
  Serial.begin(9600);
}

void loop() 
{
  printValue(0,analogRead(inputYAO21));
}
void printValue(int value,int value2)
{
  if ((millis()-lastPrintTime)< serialRefreshTime)
  {
    return;
  }

  Serial.print(value);
  Serial.print(", ");
  Serial.print(value2);
  Serial.print("\n");
  
  lastPrintTime = millis();//keep track of the last time it printed
}

