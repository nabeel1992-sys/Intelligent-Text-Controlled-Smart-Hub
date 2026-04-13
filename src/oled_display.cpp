#include "oled_display.h"

// Initialize the display object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void initOLED() {
    // S3 specific I2C initialization
    Wire.begin(I2C_SDA, I2C_SCL);
    
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("OLED allocation failed"));
        // Don't call showOLEDMessage if begin fails
        return; 
    }
    
    display.clearDisplay();
    display.display(); // Start with a clear screen
    showOLEDMessage("AURA AI HUB", "Booting system...", "Please wait");
}

void showOLEDMessage(String title, String line1, String line2) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE); // Standard define for visibility
    
    display.setCursor(0, 0);
    display.println(title);
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
    
    display.setCursor(0, 20);
    display.println(line1);
    if (line2 != "") {
        display.setCursor(0, 35);
        display.println(line2);
    }
    display.display();
}

void updateOLEDDashboard(String ip, float temp, float hum, long rssi, String uptime, bool relayState) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Top Bar (IP Address)
    display.setCursor(0, 0);
    display.print("IP: ");
    display.println(ip);
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    // Sensor Data (Formatted for clarity)
    display.setCursor(0, 15);
    display.print("Temp: "); display.print(temp, 1); display.println(" C");

    display.setCursor(0, 25);
    display.print("Hum:  "); display.print(hum, 1); display.println(" %");

    display.setCursor(0, 35);
    display.print("WiFi: "); display.print(rssi); display.println(" dBm");

    display.setCursor(0, 45);
    display.print("Time: "); display.println(uptime);

    // Bottom Bar (Hardware Status)
    display.drawLine(0, 54, 128, 54, SSD1306_WHITE);
    display.setCursor(0, 56);
    display.print("Fan (D4): ");
    display.println(relayState ? "ON" : "OFF");

    display.display();
}