#define BLYNK_TEMPLATE_ID "TMPL64AcIzWbK"
#define BLYNK_TEMPLATE_NAME "TechCultivate"
#define BLYNK_AUTH_TOKEN "0c7Pp72T-4RpfVN9GqYO_haROCKw7EgU"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Smart_Bro_0E109";
char pass[] = "DiosteBendiga08";

int RelayPin1 = 14;
int RelayPin2 = 15;
void setup() 
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  delay(1000);
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
}

void loop() 
{
  Blynk.run();
  delay(1000);
}

BLYNK_WRITE(V7) { // Replace V1 with your virtual pin number
  int relayState1 = param.asInt(); // Get the state of the Blynk switch
  digitalWrite(RelayPin1, relayState1);
}
BLYNK_WRITE(V8) { // Replace V1 with your virtual pin number
  int relayState2 = param.asInt(); // Get the state of the Blynk switch
  digitalWrite(RelayPin2, relayState2);
}