#include <Arduino.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include "wifi_config.h"
#include "web_server.h"
#include "live_data.h"
#include "oled_display.h"

#ifndef PIO_UNIT_TESTING

void setup() {
    // Initialize Serial communication for debugging
    Serial.begin(115200);

    // 🎯 1. Initialize the OLED display here
    initOLED();
    
    // Mount the internal flash memory (SPIFFS)
    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
    }

    // Read saved Wi-Fi credentials from permanent NVS memory
    Preferences prefs;
    prefs.begin("wifi", true); // Open in Read-Only mode
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    prefs.end();

    // 🎯 2. Display a "Connecting" message on the OLED screen
    showOLEDMessage("WIFI SETUP", "Connecting to:", ssid);

    // Try to connect to Wi-Fi. If no credentials exist or connection fails, start Access Point mode
    if (ssid == "" || !connectSTA(ssid, pass)) {
        startAP();
        // 🎯 3. If Wi-Fi connection fails, show status on the screen
        showOLEDMessage("WIFI FAILED", "Started AP Mode:", "ESP32_Setup");
    } else {
        // If connected successfully, print the assigned IP Address to Serial
        Serial.println();
        Serial.print("✅ Wi-Fi Connected! IP Address: ");
        Serial.println(WiFi.localIP()); 
        
        // 🎯 4. If connected, show the local IP address on the screen
        showOLEDMessage("WIFI SUCCESS", "Connected!", WiFi.localIP().toString());
    }

    // Initialize modular layers (Web Server and WebSockets)
    setupWebServer();
    setupWebSockets();
    
    // 🎯 5. Send initial system status/telemetry data to the dashboard
    sendSystemStatus();
    
    Serial.println("System Boot Complete!");
}

void loop() {
    // Let the modules handle background tasks (HTTP requests and WebSocket data)
    server.handleClient();
    handleLiveData();
}

#endif