#include "wifi_config.h"

// The actual variable is defined here
bool apMode = false;

// 1. Function to connect to the home Wi-Fi network (Station Mode)
bool connectSTA(const String& ssid, const String& pass, uint32_t timeoutMs) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
    uint32_t start = millis();
    Serial.printf("Connecting STA to SSID \"%s\" ...\n", ssid.c_str());
    
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs) {
        delay(250);
        Serial.print(".");
    }
    Serial.println();
    return WiFi.status() == WL_CONNECTED;
}

// 2. Function to create its own Hotspot (Access Point Mode)
void startAP() {
    apMode = true;
    WiFi.mode(WIFI_AP);
    const char* apSsid = "ESP32_Setup";
    const char* apPass = ""; // Open network without a password
    bool ok = WiFi.softAP(apSsid, apPass);
    delay(100);
    IPAddress ip = WiFi.softAPIP();
    Serial.printf("AP mode: SSID=%s IP=%s %s\n", apSsid, ip.toString().c_str(), ok ? "OK" : "FAIL");
}