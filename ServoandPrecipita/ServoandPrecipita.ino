#define BLYNK_TEMPLATE_ID "TMPL64AcIzWbK"
#define BLYNK_TEMPLATE_NAME "TechCultivate"
#define BLYNK_AUTH_TOKEN "0c7Pp72T-4RpfVN9GqYO_haROCKw7EgU"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Smart_Bro_0E109";
char pass[] = "DiosteBendiga08";

#define precipita 35 //rain

Servo servo1;
Servo servo2;

bool isRaining = false;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  delay(1000);

  pinMode(precipita, INPUT);

  // Initialize the servo library
  servo1.setPeriodHertz(50);  // Standard 50 Hz servo
  servo2.setPeriodHertz(50);  // Standard 50 Hz servo
  servo1.attach(5, 500, 2400);  // Attach servo 1 to pin 2, with min and max pulse width
  servo2.attach(4, 500, 2400);  // Attach servo 2 to pin 4, with min and max pulse width

  delay(1000);
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
  delay(1000);

  float Prep = analogRead(precipita);
  float precipitation = ((4095 - Prep) * 100 / 4095);
  Serial.print("Precipitation: ");
  Serial.println(precipitation);

  delay(1000);

  Blynk.virtualWrite(V4, precipitation);

  if (precipitation > 50) { // Adjust threshold as needed
    if (!isRaining) { // Check if it's just started raining
      isRaining = true;
      servo1.write(0); // Close servo 1
      servo2.write(0); // Close servo 2
    }
  } else {
    isRaining = false; // Reset the rain flag
  }
}
