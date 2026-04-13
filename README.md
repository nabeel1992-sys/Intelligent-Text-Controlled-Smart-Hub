# 🤖 Aura AI Smart Hub

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino%20(PlatformIO)-orange)
![Features](https://img.shields.io/badge/Features-OTA%20%7C%20WebSockets%20%7C%20Gemini%20AI-success)

Aura AI Smart Hub is a production-grade, full-stack Embedded IoT dashboard built on the **ESP32-S3** architecture. It features a real-time bidirectional web dashboard, live OLED hardware status, embedded Gemini AI, and a fully automated Over-The-Air (OTA) CI/CD deployment pipeline.

---

## ✨ Key Features

* **🌐 Embedded Async Web Server:** Hosts a responsive, beautifully designed HTML/CSS/JS dashboard directly from the ESP32's internal SPIFFS memory.
* **⚡ Real-Time Telemetry:** Uses **WebSockets** for zero-delay, bidirectional communication to push DHT11 sensor data (Temp/Humidity) and receive hardware control commands.
* **📺 Modular OLED Engine:** Real-time physical visualization of IP Address, Wi-Fi Signal (RSSI), Uptime, and Relay Status via an I2C OLED display.
* **☁️ Gemini AI Integration:** Chat with Google's Gemini AI directly from the local dashboard. API keys are securely managed and stored in Non-Volatile Storage (NVS).
* **🔄 Smart Wi-Fi Provisioning:** Features an auto-failover mechanism. If the saved Wi-Fi is unavailable, it automatically boots into AP (Access Point) mode (`ESP32_Setup`) to serve a captive portal for network configuration.
* **🚀 OTA & CI/CD Pipeline:** Supports local OTA updates via the browser. Additionally, a **GitHub Actions** CI/CD pipeline is integrated to automatically build and deploy firmware over-the-air upon every `git push`.
* **🛡️ Auto-Recovery UI:** The web dashboard features auto-reconnect and auto-refresh logic to seamlessly handle device reboots during OTA updates.

---

## 🛠️ Hardware Requirements

| Component | Description | Pin Mapping |
| :--- | :--- | :--- |
| **ESP32-S3** | Main Microcontroller | - |
| **DHT11 Sensor** | Temperature & Humidity Sensor | GPIO 5 |
| **SSD1306 OLED** | 0.96" I2C Display (128x64) | SDA: 6, SCL: 7 |
| **Relay / LED** | External Actuator / Indicator | GPIO 4 |

---

## 📁 Project Architecture

The codebase follows a modular C++ architecture for high maintainability:
* `main.cpp` - System boot, NVS initialization, and main super-loop.
* `wifi_config.h/.cpp` - Handles Station (STA) connection and Access Point (AP) fallback.
* `web_server.h/.cpp` - Asynchronous HTTP server, routing, and secure API key management.
* `live_data.h/.cpp` - Sensor readings, WebSockets broadcasting, and hardware control logic.
* `oled_display.h/.cpp` - Custom I2C display driver for dynamic UI pop-ups and live dashboards.
* `data/` - Contains the SPIFFS web files (`index.html`, `wifi.html`).

---

## 🚀 How to Run & Install

### 1. Prerequisites
* [VS Code](https://code.visualstudio.com/) with the [PlatformIO](https://platformio.org/) extension installed.
* ESP32-S3 board and required hardware components.

### 2. Clone and Build
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/Aura-AI-Smart-Hub.git
Open the project folder in VS Code. PlatformIO will automatically download the required libraries (WebSockets, Adafruit GFX, Adafruit SSD1306, DHT sensor library).
Connect your ESP32-S3 via USB.
3. Upload Web Files (SPIFFS)
Before flashing the firmware, you must upload the web dashboard to the ESP32's memory:
Click the PlatformIO (Alien) icon in the left sidebar.
Go to Project Tasks -> esp32s3_devkit -> Platform -> Upload Filesystem Image.
4. Upload Firmware
Build and upload the C++ code by clicking the Upload arrow at the bottom of VS Code.

--------------------------------------------------------------------------------
⚙️ Initial Setup & Usage
First Boot: The device will boot into AP mode. Connect your phone/PC to the Wi-Fi network named ESP32_Setup.
Wi-Fi Config: Navigate to http://192.168.4.1 and enter your home Wi-Fi credentials. The device will reboot and connect to your home network.
OLED Status: Check the physical OLED screen for the assigned local IP Address.
Dashboard: Open the IP Address in your browser to access the Aura AI Hub.
AI Setup: Paste your Gemini API Key in the setup box on the dashboard to activate the AI Assistant.

--------------------------------------------------------------------------------
🤖 CI/CD Automation (GitHub Actions)
This project is configured with a self-hosted runner. When a commit is pushed to the main branch, the GitHub Action automatically:
Compiles the firmware via PlatformIO.
Checks for errors.
Pushes the .bin file to the ESP32-S3 via an automated OTA web request. (The OLED display dynamically shows the "Downloading..." and "Writing to Flash..." status during this remote update).

--------------------------------------------------------------------------------
📜 License
This project is licensed under the MIT License. Feel free to use and modify it for your personal or commercial IoT projects.

---