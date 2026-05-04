# 🤖 Aura AI Smart Hub (Intelligent Text-Controlled Smart Hub)

Intelligent Text Controlled Smart Hub is a production-grade, full-stack Embedded IoT dashboard built on the **ESP32-S3** architecture.

## ✨ Key Features
* **☁️ Gemini AI Integration:** Chat with Google's Gemini AI directly from the local dashboard.
* **🚀 Cloud CI/CD Pipeline:** Automatically compiles on GitHub's Ubuntu cloud servers.

## 🛠️ Hardware Requirements & Wiring
1. **ESP32-S3 DevKit**
2. **DHT11 Sensor** -> GPIO 5
3. **0.96" OLED Display** -> I2C (SDA=6, SCL=7)
4. **Relay Module** -> GPIO 4

## 🚀 Installation & Setup

### Option 1: Using Docker (Recommended)
You don't need to install PlatformIO locally. Use the Dockerfile to generate firmware:

```bash
# Build the Docker image
docker build -t esp32-builder .

# Run the container to compile and extract firmware.bin
docker run --rm -v ${PWD}:/workspace esp32-builder
```
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
AI Chat & Hardware Control: Type commands in the chat box (e.g., "Turn on the fan") and watch the AI process the text and control the hardware instantly!
OTA Updates: To update the firmware later, just compile the code, get the firmware.bin file, and upload it via the OTA section on the web dashboard.
📜 License
This project is licensed under the MIT License. Feel free to use and modify it for your personal or commercial IoT projects.