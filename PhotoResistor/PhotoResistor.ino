#define BLYNK_TEMPLATE_ID "TMPL64AcIzWbK"
#define BLYNK_TEMPLATE_NAME "TechCultivate"
#define BLYNK_AUTH_TOKEN "0c7Pp72T-4RpfVN9GqYO_haROCKw7EgU"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Smart_Bro_0E109";
char pass[] = "DiosteBendiga08";

const int photoResistorPin = 34; //automatic light
const int ledPin = 2;           //LED light

void setup() 
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(ledPin, OUTPUT);

  delay(1000);
}
void loop(){
  Blynk.run();
  delay(1000);
  
  Serial.println();
  float lightValue = analogRead(photoResistorPin);
  float light = map(lightValue, 0, 4095, 0, 100);
  Serial.print("Light: ");
  Serial.println(light);
  if (light < 25) { 
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW); 
  }
  
  delay(100);
  Blynk.virtualWrite(V6, light);

  delay(1000);
}