#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <ESP32Servo.h>

#define BLYNK_TEMPLATE_ID "TMPL64AcIzWbK"
#define BLYNK_TEMPLATE_NAME "TechCultivate"
#define BLYNK_AUTH_TOKEN "0c7Pp72T-4RpfVN9GqYO_haROCKw7EgU"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "ALL_IS_WELL123";
char pass[] = "THE3IDIOTS";

Adafruit_BME680 bme; 

#define precipita 35 //rain
#define moisture 34 //soil
bool isRaining = false;

const int photoResistorPin = 32; //automatic light
const int ledPin = 23;           //LED light

int RelayPin1 = 14;
int RelayPin2 = 15;

const int voltageSensor = 12;
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;
int RelayPin = 13;

Servo servo1;
Servo servo2;

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

  delay(1000);

  pinMode(precipita, INPUT);
  pinMode(moisture, INPUT);
  pinMode(ledPin, OUTPUT);

  servo1.setPeriodHertz(50);  // Standard 50 Hz servo
  servo2.setPeriodHertz(50);  // Standard 50 Hz servo
  servo1.attach(5, 500, 2400);  // Attach servo 1 to pin 2, with min and max pulse width
  servo2.attach(4, 500, 2400);  // Attach servo 2 to pin 4, with min and max pulse width

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);

  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);

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
BLYNK_WRITE(V10) {  // V1 is the virtual pin for the slider in the Blynk app
  int sliderValue = param.asInt();
  
  // Map the slider value (0-255) to servo angles (0-180)
  int angle = map(sliderValue, 0, 255, 0, 180);

  // Set both servos to the same angle
  servo1.write(angle);
  servo2.write(angle);
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

  float Prep = analogRead(precipita);
  float precipitation = ((4095 - Prep) * 100 / 4095);
  Serial.print("Precipitation: ");
  Serial.println(precipitation);

  delay(1000);

  if (precipitation > 50) { // Adjust threshold as needed
    if (!isRaining) { // Check if it's just started raining
      isRaining = true;
      servo1.write(0); // Close servo 1
      servo2.write(0); // Close servo 2
    }
  } else {
    isRaining = false; // Reset the rain flag
  }
  delay(1000);

  float moist = analogRead(moisture);
  float moistures = ((4095 - moist) * 100 / 4095);
  Serial.print("Moisture: ");
  Serial.println(moistures);

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
  
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, pres);
  Blynk.virtualWrite(V3, gas);
  Blynk.virtualWrite(V4, precipitation);
  Blynk.virtualWrite(V5, moistures);
  Blynk.virtualWrite(V6, light);
  Blynk.virtualWrite(V9, vIN);


  Serial.println();
  delay(2000);
}
