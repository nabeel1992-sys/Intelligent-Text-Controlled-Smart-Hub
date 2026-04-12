#include <Arduino.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include "wifi_config.h"
#include "web_server.h"
#include "live_data.h"

#ifndef PIO_UNIT_TESTING

void setup() {
    Serial.begin(115200);
    
    // Mount the internal memory (SPIFFS)
    SPIFFS.begin(true);

    // Read saved Wi-Fi credentials from permanent memory
    Preferences prefs;
    prefs.begin("wifi", true);
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    prefs.end();

    // Try to connect to Wi-Fi. If it fails, start the Setup Hotspot
    if (ssid == "" || !connectSTA(ssid, pass)) {
        startAP();
    } else {
        // Agar connect ho jaye toh IP Address print karo
        Serial.println();
        Serial.print("✅ Wi-Fi Connected! IP Address: ");
        Serial.println(WiFi.localIP()); 
    }

    // Initialize our modular layers
    setupWebServer();
    setupWebSockets();
    Serial.println("System Boot Complete!");
}

void loop() {
    // Let the modules do their infinite background jobs
    server.handleClient();
    handleLiveData();
}
#endif