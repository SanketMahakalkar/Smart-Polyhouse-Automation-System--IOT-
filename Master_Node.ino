/*
 * Project: Smart Polyhouse Automation (Master Node)
 * Board: ESP8266 (NodeMCU)
 * Function: Receives sensor data via NRF24L01 and uploads to Firebase.
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "FirebaseESP8266.h"  // Firebase ESP8266 library
#include <ESP8266WiFi.h>

// --- CONFIGURATION ---
// IMPORTANT: Replace these with your actual credentials if testing locally.
// Do NOT upload real passwords to public GitHub repositories.
#define FIREBASE_HOST "polyhouse-11bca-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "YOUR_FIREBASE_DATABASE_SECRET" 
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Define Firebase Data object
FirebaseData firebaseData;

// Data Structure to match the Slave Node
struct MyData {
  float hum;
  float temp;
};
MyData data;

// NRF24L01 Configuration
RF24 radio(2, 4); // CE, CSN Pins
const byte address[6] = "00002"; // Pipe Address

void setup() {
  Serial.begin(9600);

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
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void sensorUpdate() {
  // Upload Temperature
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", data.temp)) {
    Serial.println("Temp Upload: PASSED");
  } else {
    Serial.print("Temp Upload: FAILED. Reason: ");
    Serial.println(firebaseData.errorReason());
  }

  // Upload Humidity
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", data.hum)) {
    Serial.println("Hum Upload: PASSED");
  } else {
    Serial.print("Hum Upload: FAILED. Reason: ");
    Serial.println(firebaseData.errorReason());
  }
}

void loop() {
  // Check for incoming data from Slave Node
  if (radio.available()) {
    radio.read(&data, sizeof(MyData));
    
    // Print to Serial Monitor for Debugging
    Serial.print("Humidity: ");
    Serial.print(data.hum);
    Serial.print(" %, Temp: ");
    Serial.print(data.temp);
    Serial.println(" Celsius");

    // Upload to Cloud
    sensorUpdate();
  }
}
