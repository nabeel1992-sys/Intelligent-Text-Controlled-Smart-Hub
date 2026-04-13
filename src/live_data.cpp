#include "live_data.h"
#include "oled_display.h"
#include <WiFi.h>
#include <DHT.h> 

// --- DHT11 Sensor Setup ---
#define DHTPIN 5      // GPIO pin for DHT11
#define DHTTYPE DHT11 // Sensor type
DHT dht(DHTPIN, DHTTYPE); 

// Global variables for tracking state
float lastTemp = 0;
float lastHum = 0;
int lastRSSI = 0;
String lastUptime = "";
unsigned long lastSensorMillis = 0;

// Create the WebSockets server on port 81
WebSocketsServer webSocket(81);

// Macro for External LED / Relay Pin
#define EXTERNAL_LED 4

// Function to calculate device uptime
String getFormattedUptime() {
    unsigned long seconds = millis() / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    seconds %= 60;
    minutes %= 60;
    char buffer[16];
    sprintf(buffer, "%02lu:%02lu:%02lu", hours, minutes, seconds);
    return String(buffer);
}

// Function to send logs to Serial Monitor and Web Dashboard
void broadcastLog(const String& msg) {
    Serial.println(msg); 
    webSocket.broadcastTXT(msg); 
}

// Function to blink LED twice for confirmation
void blinkConfirm() {
    for (int i = 0; i < 2; i++) {
        digitalWrite(EXTERNAL_LED, HIGH);
        delay(100);
        digitalWrite(EXTERNAL_LED, LOW);
        delay(100);
    }
}

// Function to read DHT11 and update both Web and OLED
void sendSystemStatus() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int r = WiFi.RSSI();
    String up = getFormattedUptime();

    // Check if sensor reading failed
    if (isnan(t) || isnan(h)) {
        Serial.println("❌ Failed to read from DHT sensor!");
        return; 
    }

    // Update variables for tracking
    lastTemp = t;
    lastHum = h;
    lastRSSI = r;
    lastUptime = up;
    
    // Create a simple log string for the web dashboard
    // Note: Using String() for floats to ensure ESP32 compatibility
    String logMsg = "🌡 Temp: " + String(t, 1) + "C | 💧 Hum: " + String(h, 0) + "% | 📶 RSSI: " + String(r) + "dBm";
    broadcastLog(logMsg);

    // Update the OLED screen with the same data
    updateOLEDDashboard(WiFi.localIP().toString(), t, h, r, up, digitalRead(EXTERNAL_LED));
}

// Event handler for incoming WebSocket messages (Commands from Dashboard)
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_TEXT) {
        String msg = (char*)payload;
        
        if (msg == "led_on") {
            digitalWrite(EXTERNAL_LED, HIGH);
            broadcastLog("💡 Command: LED ON");
            blinkConfirm();
        }
        else if (msg == "led_off") {
            digitalWrite(EXTERNAL_LED, LOW);
            broadcastLog("💡 Command: LED OFF");
            blinkConfirm();
        }
        else if (msg == "reboot") {
            broadcastLog("🔄 System Rebooting...");
            delay(1000);
            ESP.restart();
        }
        
        // Push an immediate update after any command
        sendSystemStatus();
    }
}

// Initialize WebSockets, Hardware pins, and Sensors
void setupWebSockets() {
    pinMode(EXTERNAL_LED, OUTPUT);
    digitalWrite(EXTERNAL_LED, LOW); 
    
    dht.begin(); 
    webSocket.begin(); 
    webSocket.onEvent(webSocketEvent);
    Serial.println("✅ Live Data System Ready");
}

// Main loop function (Called from main.cpp)
void handleLiveData() {
    webSocket.loop(); 
    
    unsigned long currentMillis = millis();
    // Send data update every 15 seconds
    if (currentMillis - lastSensorMillis >= 20000) {
        lastSensorMillis = currentMillis;
        sendSystemStatus();
    }
}