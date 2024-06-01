#define BLYNK_TEMPLATE_ID "TMPL64AcIzWbK"
#define BLYNK_TEMPLATE_NAME "TechCultivate"
#define BLYNK_AUTH_TOKEN "0c7Pp72T-4RpfVN9GqYO_haROCKw7EgU"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Smart_Bro_0E109";
char pass[] = "DiosteBendiga08";

#define precipita 35 //rain
#define moisture 32 //soil

void setup(){
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  delay(1000);

  pinMode(precipita, INPUT);
  pinMode(moisture, INPUT);

  delay(1000);
}

void loop(){
  Blynk.run();
  delay(1000);

  Serial.println();
  float Prep = analogRead(precipita);
  float moist = analogRead(moisture);
  float precipitation = ((4095 - Prep) * 100 / 4095);
  float moistures = ((4095 - moist) * 100 / 4095);
  Serial.print("Precipitation: ");
  Serial.println(precipitation);
  Serial.print("Moisture: ");
  Serial.println(moistures); 
  
  delay(1000);

  Blynk.virtualWrite(V4, precipitation);
  Blynk.virtualWrite(V5, moistures);

}