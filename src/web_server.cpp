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
        File f = SPIFFS.open("/wifi.html", "r");
        if (!f) {
            server.send(500, "text/plain", "wifi.html missing");
            return;
        }
        server.streamFile(f, "text/html");
        f.close();
    } else {
        // In Station mode, serve the main dashboard
        File f = SPIFFS.open("/index.html", "r");
        if (!f) {
            server.send(500, "text/plain", "index.html missing");
            return;
        }
        server.streamFile(f, "text/html");
        f.close();
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
    
    server.send(200, "text/plain", "Saved. Rebooting...");
    broadcastLog("Wi-Fi credentials saved. Rebooting...");
    delay(500);
    ESP.restart();
}

// 3. Function to setup all URL routes and initialize the OTA system
void setupWebServer() {
    // Dynamic root route depending on the current Wi-Fi mode
    server.on("/", HTTP_GET, handleRoot);
    
    // OTA firmware upload routes
    server.on("/update", HTTP_POST, []() {
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
    
    // Route to save credentials (called by wifi.html)
    server.on("/save", HTTP_POST, handleSaveWifi);
    
    // Handle 404 Not Found (redirect to portal if in AP mode)
    server.onNotFound([]() {
        if (apMode) {
            handleRoot();
        } else {
            server.send(404, "text/plain", "Not found");
        }
    });
    
    // Start the web server
    server.begin();
}