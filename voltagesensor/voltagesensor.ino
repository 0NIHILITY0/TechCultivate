const int voltageSensor = 12;
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;

int RelayPin = 13;
void setup() 
{
  Serial.begin(9600);
  delay(1000);
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);
}

void loop() 
{

  value = analogRead(voltageSensor);
  vOUT = (value * 3.3) / 1024.0;
  vIN = ((vOUT / (R2/(R1+R2)))/3.3) - 3.5;
  Serial.print("Input Solar Voltage: ");
  Serial.println(vIN);
  delay(1000);

 if (vIN>= 4)
 {
  digitalWrite(RelayPin, LOW);
  delay(100);
 }
  if (vIN<=5)
  {
   digitalWrite(RelayPin, HIGH);
   delay(100);    
  }
}