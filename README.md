# ESP32 Smart Home Automation System

## Overview
This project is an ESP32-based smart home automation system integrated with a Telegram Bot for remote monitoring and control. The system supports manual and automatic operation modes, environmental sensing, lighting automation, servo-based window control, and real-time digital clock display using an NTP server.

Designed as an academic IoT project, this repository demonstrates the implementation of embedded systems, sensor integration, and real-time communication using ESP32.

---

## Main Features
- Telegram Bot remote control and monitoring
- Dual operation modes:
  - Manual Mode
  - Automatic Mode
- Automatic lighting and window control based on LDR sensor
- Temperature and humidity monitoring using DHT11
- Servo motor control for window automation
- Digital clock display via NTP server
- Seven-segment display using TM1637
- Real-time serial monitoring

---

## Hardware Components
- ESP32 Development Board
- 3 LEDs
- LDR Sensor
- DHT11 Temperature & Humidity Sensor
- Servo Motor
- TM1637 4-Digit Seven Segment Display
- Breadboard and Jumper Wires
- Stable Power Supply

---

## Pin Configuration
| Component | ESP32 Pin |
|----------|-----------|
| LED1 | 25 |
| LED2 | 26 |
| LED3 | 27 |
| LDR Sensor | 34 |
| Servo Motor | 13 |
| DHT11 Sensor | 14 |
| TM1637 CLK | 18 |
| TM1637 DIO | 19 |

---

## Software Requirements
- Arduino IDE
- ESP32 Board Package
- Required Libraries:
  - WiFi.h
  - WiFiClientSecure.h
  - UniversalTelegramBot
  - ArduinoJson
  - ESP32Servo
  - DHT Sensor Library
  - TM1637Display
  - time.h

---

## System Architecture
The system connects to WiFi and communicates with a Telegram Bot via secure client connection. User commands from Telegram are processed by ESP32 to control LEDs, servo motor, and sensor monitoring.

In automatic mode:
- LDR sensor determines lighting conditions
- LEDs and window servo adjust automatically

In manual mode:
- User directly controls devices via Telegram commands

---

## Operation Flow
1. ESP32 connects to WiFi network
2. System synchronizes time using NTP server
3. Telegram bot waits for user commands
4. User selects operation mode:
   - Manual
   - Automatic
5. Devices operate based on selected mode
6. Seven segment display continuously shows real-time clock

---

## Telegram Commands

### Mode Selection
- `/start` – Initialize system and select operation mode
- `Manual` – Activate manual mode
- `Automatic` – Activate automatic mode

### Lighting Control
- `/LED1_ON` `/LED1_OFF`
- `/LED2_ON` `/LED2_OFF`
- `/LED3_ON` `/LED3_OFF`

### Window Control
- `/open_window`
- `/close_window`

### Sensor Monitoring
- `/check_temperature`
- `/check_humidity`


---

## Installation Steps
1. Clone this repository https://github.com/harsadella/esp32-smart-home-telegram
2. Open the project in Arduino IDE
3. Install all required libraries
4. Configure:
- WIFI_SSID
- WIFI_PASSWORD
- BOT_TOKEN
5. Select ESP32 Board
6. Upload code to ESP32

---

## Project Purpose
This project was developed as an academic IoT and embedded systems implementation to demonstrate:
- Sensor integration
- Automation logic
- Remote control via messaging platform
- Real-time embedded system monitoring

---

## License
This project is intended for educational and learning purposes.

