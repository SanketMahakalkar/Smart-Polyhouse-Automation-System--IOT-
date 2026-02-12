/*
 * Project: Smart Polyhouse Automation (Master Node)
 * Board: ESP8266 (NodeMCU)
 * Function: 
 * 1. Receives sensor data via NRF24L01.
 * 2. Uploads data to Firebase.
 * 3. Controls Relays (Lights, Cooler, Pump) based on Firebase commands.
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

// --- CREDENTIALS (DO NOT SHARE ON GITHUB) ---
#define FIREBASE_HOST "polyhouse-11bca-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "YOUR_FIREBASE_SECRET_KEY" // Replace with real key for testing
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// --- HARDWARE CONFIGURATION ---
// Relay Pins
const int PIN_LIGHTS = D0;
const int PIN_COOLER = D1;
const int PIN_PUMP   = D3;

// NRF24L01 Pins (CE, CSN)
RF24 radio(2, 4); 
const byte address[6] = "00002";

// Firebase Objects
FirebaseData firebaseData;
FirebaseData lightsData, coolerData, pumpData;

// Data Structure (Must match Slave Node)
struct MyData {
  float hum;
  float temp;
  int soil_moisture; // 'val' in your original code
};
MyData data;

void setup() {
  Serial.begin(9600);

  // Initialize Relay Pins
  pinMode(PIN_LIGHTS, OUTPUT);
  pinMode(PIN_COOLER, OUTPUT);
  pinMode(PIN_PUMP, OUTPUT);
  
  // Set default state to OFF
  digitalWrite(PIN_LIGHTS, LOW);
  digitalWrite(PIN_COOLER, LOW);
  digitalWrite(PIN_PUMP, LOW);

  // Initialize NRF24L01
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  // Initialize WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected. IP: " + WiFi.localIP().toString());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void uploadSensorData() {
  // Upload Temperature
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", data.temp)) {
    Serial.println("Temp Upload: PASSED");
  } else {
    Serial.println("Temp Upload: FAILED - " + firebaseData.errorReason());
  }

  // Upload Humidity
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", data.hum)) {
    Serial.println("Hum Upload: PASSED");
  } else {
    Serial.println("Hum Upload: FAILED - " + firebaseData.errorReason());
  }

  // Upload Soil Moisture
  if (Firebase.setInt(firebaseData, "/FirebaseIOT/soilmoisture", data.soil_moisture)) {
    Serial.println("Soil Moisture Upload: PASSED");
  } else {
    Serial.println("Soil Moisture Upload: FAILED - " + firebaseData.errorReason());
  }
}

void checkRelayCommands() {
  // 1. Check Lights
  if (Firebase.getString(lightsData, "/FirebaseIOT/lights1")) {
    if (lightsData.stringData() == "1") digitalWrite(PIN_LIGHTS, HIGH);
    else digitalWrite(PIN_LIGHTS, LOW);
  }

  // 2. Check Cooler
  if (Firebase.getString(coolerData, "/FirebaseIOT/cooler1")) {
    if (coolerData.stringData() == "1") digitalWrite(PIN_COOLER, HIGH);
    else digitalWrite(PIN_COOLER, LOW);
  }

  // 3. Check Water Pump
  if (Firebase.getString(pumpData, "/FirebaseIOT/watersupply1")) {
    if (pumpData.stringData() == "1") digitalWrite(PIN_PUMP, HIGH);
    else digitalWrite(PIN_PUMP, LOW);
  }
}

void loop() {
  // 1. Receive Data from Slave
  if (radio.available()) {
    radio.read(&data, sizeof(MyData));
    
    // Debug Print
    Serial.print("H: "); Serial.print(data.hum);
    Serial.print("%, T: "); Serial.print(data.temp);
    Serial.print("C, Soil: "); Serial.println(data.soil_moisture);

    // Upload ONLY when new data arrives
    uploadSensorData();
  }

  // 2. Check for App Commands (Relays)
  checkRelayCommands();
  
  delay(100); // Small delay for stability
}
