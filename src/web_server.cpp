#include "web_server.h"
#include "wifi_config.h" // Included to access the apMode variable
#include <Update.h>
#include <SPIFFS.h>
#include <Preferences.h>

// Create the WebServer object on port 80
WebServer server(80);

// We tell the compiler that these functions exist in other files (Layer 3 & main)
extern void broadcastLog(const String& msg);
extern void blinkConfirm();

// 1. Function to handle the root URL ("/")
void handleRoot() {
    if (apMode) {
        // In AP mode, serve the Wi-Fi setup portal
        File file = SPIFFS.open("/wifi.html", "r");
        server.streamFile(file, "text/html");
        file.close();
    } else {
        File file = SPIFFS.open("/index.html", "r");
        server.streamFile(file, "text/html");
        file.close();
    }
}

// 2. Function to save new Wi-Fi credentials
void handleSaveWifi() {
    if (server.method() != HTTP_POST) {
        server.send(405, "text/plain", "Method Not Allowed");
        return;
    }
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");

    if (ssid.length() == 0) {
        server.send(400, "text/plain", "SSID required");
        return;
    }

    Preferences prefs;
    prefs.begin("wifi", false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.end();

    server.send(200, "text/html", "<h1>Saved! Rebooting...</h1>");
    delay(2000);
    ESP.restart();
}

// 3. Function to save Gemini API Key to Flash Memory
void handleSaveApiKey() {
    if (server.method() != HTTP_POST) {
        server.send(405, "text/plain", "Method Not Allowed");
        return;
    }
    String apiKey = server.arg("apikey");
    
    Preferences prefs;
    prefs.begin("ai_settings", false); // false = Read/Write mode
    prefs.putString("gemini_key", apiKey);
    prefs.end();
    
    server.send(200, "text/plain", "API Key Saved Securely!");
    broadcastLog("🔑 API Key securely saved in ESP32 Flash Memory!");
}

// 4. Function to retrieve Gemini API Key from Flash Memory
void handleGetApiKey() {
    Preferences prefs;
    prefs.begin("ai_settings", true); // true = Read Only mode
    String key = prefs.getString("gemini_key", "");
    prefs.end();
    
    server.send(200, "text/plain", key);
}

// 5. Main Setup Function for Web Server
void setupWebServer() {
    // Serve static files
    server.on("/", HTTP_GET, handleRoot);

    // OTA Update Routes
    server.on("/update", HTTP_POST, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        blinkConfirm();
        ESP.restart();
    }, []() {
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
            Serial.printf("Starting OTA: %s\n", upload.filename.c_str());
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) {
                Serial.printf("OTA done: %u bytes\n", upload.totalSize);
            } else {
                Update.printError(Serial);
            }
        }
    });

    // Route to save Wi-Fi credentials (called by wifi.html)
    server.on("/save", HTTP_POST, handleSaveWifi);

    // NAYE ROUTES: Secure API Key handling
    server.on("/save-api", HTTP_POST, handleSaveApiKey);
    server.on("/get-api", HTTP_GET, handleGetApiKey);

    // Handle 404 Not Found (redirect to portal if in AP mode)
    server.onNotFound([]() {
        if (apMode) {
            handleRoot();
        } else {
            server.send(404, "text/plain", "Not found");
        }
    });

    server.begin();
}