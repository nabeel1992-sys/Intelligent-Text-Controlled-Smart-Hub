#include "live_data.h"
#include "oled_display.h"
#include <WiFi.h>
#include <DHT.h> // Include the DHT library

// --- DHT11 Sensor Setup ---
#define DHTPIN 5      // The GPIO pin connected to DHT11 DATA (e.g., GPIO 5)
#define DHTTYPE DHT11 // Define the sensor type as DHT11
DHT dht(DHTPIN, DHTTYPE); // Create the DHT object
float lastHum = 0;    // Variable to store the last humidity

// Create the WebSockets server on port 81
WebSocketsServer webSocket(81);

// 🎯 Macro for External LED / Relay Pin
#define EXTERNAL_LED 4

// Variables for telemetry timers
unsigned long lastSensorMillis = 0;
float lastTemp = 0;
int lastRSSI = 0;
String lastUptime = "";

// Function to calculate device uptime
String getFormattedUptime() {
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  seconds %= 60;
  minutes %= 60;
  char buffer[16]; // Safe buffer size
  sprintf(buffer, "%02lu:%02lu:%02lu", hours, minutes, seconds);
  return String(buffer);
}

// Function to send logs to Serial Monitor and Web Dashboard
void broadcastLog(const String& msg) {
  Serial.println(msg); // Show log in Serial Monitor
  String tempMsg = msg; // Copy to fix the const issue
  webSocket.broadcastTXT(tempMsg); // Send log to the Web Dashboard
}

// Function to blink LED twice for confirmation
void blinkConfirm() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(EXTERNAL_LED, HIGH); // Active-High (ON)
    delay(100);
    digitalWrite(EXTERNAL_LED, LOW);  // Active-High (OFF)
    delay(100);
  }
}

// Function to read DHT11 and send telemetry to dashboard
void sendSystemStatus() {
  // Read Temperature and Humidity from DHT11
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int r = WiFi.RSSI();
  String up = getFormattedUptime();

  // Check if sensor reading failed (NaN means Not a Number)
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor! Check Wiring.");
    return; // Stop the function here if reading fails
  }

  // Send data only if something has changed (to save Wi-Fi bandwidth)
  if (t != lastTemp || h != lastHum || r != lastRSSI || up != lastUptime) {
    lastTemp = t;
    lastHum = h;
    lastRSSI = r;
    lastUptime = up;
    
    char buffer[128]; // Safe buffer size for long text
    // Combine all data into one beautiful text string
    sprintf(buffer, "🌡 Temp: %.1f°C | 💧 Hum: %.1f%% | 📶 RSSI: %d dBm | ⏱ Uptime: %s", t, h, r, up.c_str());
    broadcastLog(String(buffer)); // Send to browser

    // Send data to modular OLED display
    updateOLEDDashboard(WiFi.localIP().toString(), t, h, r, up, digitalRead(EXTERNAL_LED));
  }
}

// Event handler for incoming WebSocket messages from the browser
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = (char*)payload; // Convert payload to String
    
    if (msg == "led_on") {
      digitalWrite(EXTERNAL_LED, HIGH); // Turn LED ON (Active-High logic)
      broadcastLog("💡 Command received: LED ON");
    }
    else if (msg == "led_off") {
      digitalWrite(EXTERNAL_LED, LOW); // Turn LED OFF (Active-High logic)
      broadcastLog("💡 Command received: LED OFF");
    }
    else if (msg == "reboot") {
      broadcastLog("🔄 Rebooting device...");
      delay(500);
      ESP.restart(); // Restart the ESP32
    }
  }
}

// Initialize WebSockets, Hardware pins, and Sensors
void setupWebSockets() {
  pinMode(EXTERNAL_LED, OUTPUT);
  digitalWrite(EXTERNAL_LED, LOW); // Set LED OFF initially
  
  dht.begin(); // Start the DHT sensor
  webSocket.begin(); // Start the WebSocket server
  webSocket.onEvent(webSocketEvent); // Tell server where to send incoming messages
}

// Main loop function for live data
void handleLiveData() {
  webSocket.loop(); // Keep WebSocket connection alive
  
  // Timer to send telemetry every 20 seconds without blocking the code (No delay)
  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorMillis >= 20000) {
    lastSensorMillis = currentMillis;
    sendSystemStatus();
  }
}
