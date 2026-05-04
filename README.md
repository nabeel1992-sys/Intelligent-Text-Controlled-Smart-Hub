# 🤖 Intelligent Text-Controlled Smart Hub

Intelligent Text Controlled Smart Hub is a production-grade, full-stack Embedded IoT dashboard built on the **ESP32-S3** architecture. It features a real-time bidirectional web dashboard, live OLED hardware status, embedded Gemini AI, and a fully automated Cloud CI/CD deployment pipeline using Docker and GitHub Actions.

## ✨ Key Features

* **☁️ Gemini AI Integration:** Chat with Google's Gemini AI directly from the local dashboard. API keys are securely managed and stored in the ESP32's Non-Volatile Storage (NVS).
* **🌐 Embedded Async Web Server:** Hosts a responsive, beautifully designed HTML/CSS/JS dashboard directly from the ESP32's internal SPIFFS memory.
* **⚡ Real-Time Telemetry:** Uses **WebSockets** for zero-delay, bidirectional communication to push DHT11 sensor data (Temperature/Humidity) and receive hardware control commands.
* **📺 Modular OLED Engine:** Real-time physical visualization of IP Address, Wi-Fi Signal (RSSI), Uptime, and Relay/Fan Status via a 128x64 I2C OLED display.
* **🔄 Smart Wi-Fi Provisioning:** Features an auto-failover mechanism. If the saved Wi-Fi is unavailable, it automatically boots into AP (Access Point) mode (`ESP32_Setup`) to serve a captive portal for network configuration.
* **🚀 Cloud CI/CD Pipeline (GitHub Actions):** Automatically compiles the firmware on GitHub's Ubuntu cloud servers upon every `git push` to ensure code integrity and generate the `.bin` artifact.
* **📲 OTA (Over-The-Air) Updates:** Securely flash new `.bin` firmware files directly from the web dashboard without needing a USB cable. The UI features auto-reconnect and auto-refresh logic to seamlessly handle device reboots.
* **🐳 Docker Support:** Includes a lightweight `Dockerfile` based on `python:3.10-slim` for clean, isolated, and consistent local PlatformIO builds without installing heavy toolchains.

## 🛠️ Hardware Requirements & Wiring

1. **ESP32-S3 DevKit** (e.g., `esp32-s3-devkitc-1`)
2. **DHT11 Sensor** -> Data pin connected to **GPIO 5**
3. **0.96" OLED Display (SSD1306)** -> I2C Pins: SDA = **GPIO 6**, SCL = **GPIO 7**
4. **External LED / Relay Module** -> Connected to **GPIO 4**

## 🚀 Installation & Setup

### Option 1: Using Docker (Recommended for clean builds)
You don't need to install PlatformIO locally on your machine. Just use the included Dockerfile to generate the firmware:
```bash
# Build the Docker image
docker build -t esp32-builder .

# Run the container to compile and extract firmware.bin
docker run --rm -v ${PWD}:/workspace esp32-builder
Option 2: Using VS Code & PlatformIO
Install VS Code and the PlatformIO extension.
Clone this repository and open the folder in VS Code.
PlatformIO will automatically install the required libraries (WebSockets, DHT Sensor, Adafruit GFX, SSD1306, Unity).
Connect your ESP32-S3 via USB for the first time.
Click Build and Upload. (Important: Make sure to run "Upload File System Image" as well to load the HTML/CSS files into SPIFFS!)
🌐 Web Interface & Usage
First Boot (Captive Portal): If the ESP32 cannot find a known Wi-Fi network, connect to the hotspot named ESP32_Setup and navigate to http://192.168.4.1 to enter your home Wi-Fi credentials.
Dashboard Access: Once connected, the OLED screen will display the assigned local IP address. Open this IP in your browser to access the Aura AI Hub dashboard.
API Key Setup: Paste your Gemini API key in the setup box on the dashboard to enable AI functionalities.
AI Chat & Hardware Control: Type commands in the chat box (e.g., "Turn on the fan") and watch the AI process the text and control the hardware instantly.
OTA Updates: To update the firmware later, just compile the code, get the firmware.bin file, and upload it via the OTA section on the web dashboard.
📜 License
This project is licensed under the MIT License. Feel free to use and modify it for your personal or commercial IoT projects.