#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BLYNK_TEMPLATE_ID "TMPL64AcIzWbK"
#define BLYNK_TEMPLATE_NAME "TechCultivate"
#define BLYNK_AUTH_TOKEN "0c7Pp72T-4RpfVN9GqYO_haROCKw7EgU"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Smart_Bro_0E109";
char pass[] = "DiosteBendiga08";

Adafruit_BME680 bme; 

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass); 
  delay(1000);
  Wire.begin();

  while (!Serial);
  Serial.println(F("BME680 async test"));

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {
  Blynk.run();

  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  Serial.print(F("Reading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.println(endTime);

  Serial.println(F("You can do other work during BME680 measurement."));
  delay(50); 

  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(bme.temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(bme.pressure / 100.0);
  Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(bme.humidity);
  Serial.println(F(" %"));

  Serial.print(F("Gas = "));
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(F(" KOhms"));

  float temp = bme.temperature;
  float pres = bme.pressure / 100.0;
  float hum = bme.humidity;
  float gas = bme.gas_resistance / 1000.0;

  delay(1000);
  
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V2, pres);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V3, gas);


  Serial.println();
  delay(2000);
}