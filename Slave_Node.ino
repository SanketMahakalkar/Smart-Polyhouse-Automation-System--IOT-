/*
 * Project: Smart Polyhouse Automation (Slave Node)
 * Board: Arduino Uno / Nano
 * Function: Reads DHT22 (Temp/Hum) & Soil Moisture, then sends via NRF24L01.
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

// --- HARDWARE CONFIGURATION ---
#define DHTPIN 2      // DHT22 Data Pin
#define DHTTYPE DHT22 // Sensor Type
#define SENSOR_POWER_PIN 6 // Power pin for Soil Moisture Sensor (prevents corrosion)
#define SENSOR_PIN A0      // Analog Output from Soil Moisture Sensor

// NRF24L01 Configuration (CE=7, CSN=8)
RF24 radio(7, 8); 
const byte address[6] = "00002"; 

// Initialize DHT
DHT dht(DHTPIN, DHTTYPE);

// Data Structure (MUST MATCH MASTER NODE EXACTLY)
struct MyData {
  float hum;
  float temp;
  int soil_moisture; // Added Soil Moisture value
};
MyData data;

void setup() {
  Serial.begin(9600);
  
  // Configure Soil Moisture Power Pin
  pinMode(SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR_POWER_PIN, LOW); // Keep it OFF initially

  // Initialize NRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening(); 

  // Initialize DHT
  dht.begin();
}

void loop() {
  // 1. Read Temperature & Humidity
  data.hum = dht.readHumidity();
  data.temp = dht.readTemperature();

  // 2. Read Soil Moisture
  data.soil_moisture = readSoilSensor();

  // 3. Debugging: Print to Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(data.hum);
  Serial.print(" %, Temp: ");
  Serial.print(data.temp);
  Serial.print(" C, Soil Moisture: ");
  Serial.println(data.soil_moisture); // 0-1023 (Lower is usually wetter)

  // 4. Send Data
  radio.write(&data, sizeof(MyData));

  delay(1000);
}

// Function to read Soil Moisture safely (prevents sensor corrosion)
int readSoilSensor() {
  digitalWrite(SENSOR_POWER_PIN, HIGH); // Turn sensor ON
  delay(10);                            // Wait for power to settle
  int val = analogRead(SENSOR_PIN);     // Read Analog value (0-1023)
  digitalWrite(SENSOR_POWER_PIN, LOW);  // Turn sensor OFF
  return val;
}
