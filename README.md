# 🌱 Smart Polyhouse Automation System (IoT)

A wireless, IoT-enabled environmental monitoring and control system designed for precision agriculture. This project utilizes a **Master-Slave architecture** to monitor soil moisture, temperature, and humidity across multiple nodes and syncs data to the cloud for remote management via an Android App.

---

## 🚀 Key Features
* **Wireless Sensor Network:** Implements a Master-Slave topology using **NRF24L01** transceivers (SPI Protocol).
* **Real-Time Cloud Logging:** Syncs sensor data to **Google Firebase Realtime Database** via ESP8266 (Wi-Fi).
* **Remote Control:** Custom **Android App** (MIT App Inventor) allows users to toggle pumps, fans, and lights remotely.
* **Automated Thresholds:** System automatically triggers irrigation or ventilation based on pre-set sensor values.
* **Custom PCB Design:** Schematics and power distribution designed using **EasyEDA**.

---

## 🛠️ System Architecture

The system consists of three main layers:
1.  **Slave Nodes (Sensor Layer):** Arduino Nano + Sensors (DHT22, Soil Moisture) collect data and transmit it wirelessly via NRF24L01.
2.  **Master Node (Gateway Layer):** Receives data from Slave nodes, processes it, and sends it to the Cloud via ESP8266 (UART Communication).
3.  **Cloud & User Interface:** Google Firebase stores the data, which is visualized and controlled via the Android App.

### Hardware Tech Stack
* **Microcontrollers:** Arduino Uno/Nano (ATmega328P), ESP8266 (Wi-Fi Module).
* **Communication:** NRF24L01 (2.4GHz RF), UART (Serial), SPI, I2C.
* **Sensors:** DHT22 (Temp/Humidity), Capacitive Soil Moisture Sensor.
* **Actuators:** 5V Relay Modules (for Water Pumps and Exhaust Fans).

### Software Tech Stack
* **Firmware:** Embedded C++ (Arduino IDE).
* **Cloud:** Google Firebase Realtime Database.
* **Mobile App:** MIT App Inventor (Block-based coding).
* **PCB Design:** EasyEDA.

---


| Component | Arduino Pin | Protocol |
| :--- | :--- | :--- |
| **NRF24L01** | 9, 10, 11, 12, 13 | SPI |
| **ESP8266** | 2, 3 (SoftSerial) | UART |
| **DHT22** | 4 | One-Wire |
| **Relay** | 5, 6 | GPIO |

---


---

## 🔮 Future Improvements
* Integration of **LoRaWAN** for longer-range communication (km range).
* Implementation of **PID Control** for more precise temperature regulation.
* Solar power integration for off-grid Slave nodes.

---

## 👨‍💻 Author
**Sanket Mahakalkar**
* [LinkedIn](https://linkedin.com/in/sanket-mahakalkar)
* [GitHub](https://github.com/SanketMahakalkar)
