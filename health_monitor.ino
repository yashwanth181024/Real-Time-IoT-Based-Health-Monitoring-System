/*
  Real-Time IoT-Based Health Monitoring System
  ---------------------------------------------
  Reads BPM from a PPG pulse sensor and temperature from a DHT11 sensor,
  displays both on a 16x2 LCD, and streams CSV-formatted readings over
  USB serial (9600 baud) for logging on a connected PC.

  Frame format sent over serial:
    Time_ms,BPM,Temperature_C

  Hardware:
    Pulse Sensor Signal -> A0
    DHT11 Data           -> D10
    16x2 LCD             -> RS, E, D4-D7 on digital pins (4-bit mode)
*/

#include <PulseSensorPlayground.h>
#include "DHT.h"
#include <LiquidCrystal.h>

// ---- Pulse Sensor ----
#define PULSE_PIN A0

// ---- DHT11 ----
#define DHT_PIN 10
#define DHT_TYPE DHT11

// ---- LCD (RS, E, D4, D5, D6, D7) ----
// Adjust these pin numbers to match your actual wiring.
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

PulseSensorPlayground pulseSensor;
DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastReading = 0;
int currentBPM = 0;
float currentTemperature = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();

  lcd.begin(16, 2);
  lcd.print("Health Monitor");
  delay(1000);
  lcd.clear();

  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.setThreshold(550);

  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor Started");
  }

  // CSV header
  Serial.println("Time_ms,BPM,Temperature_C");
}

void loop() {
  // Get BPM
  int bpm = pulseSensor.getBeatsPerMinute();

  if (pulseSensor.sawStartOfBeat()) {
    currentBPM = bpm;
  }

  // Read and report every 2 seconds
  if (millis() - lastReading >= 2000) {
    lastReading = millis();

    float temperature = dht.readTemperature();
    if (!isnan(temperature)) {
      currentTemperature = temperature;
    }

    // Update LCD
    lcd.setCursor(0, 0);
    lcd.print("BPM: ");
    lcd.print(currentBPM);
    lcd.print("    "); // clear trailing chars

    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(currentTemperature);
    lcd.print(" C ");

    // CSV format over serial
    Serial.print(millis());
    Serial.print(",");
    Serial.print(currentBPM);
    Serial.print(",");
    Serial.println(currentTemperature);
  }

  delay(20);
}
