#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Custom I2C Pins for ESP32-S3
#define I2C_SDA 6
#define I2C_SCL 7

// OLED Functions
void initOLED();
void showOLEDMessage(String title, String line1, String line2 = "");
void updateOLEDDashboard(String ip, float temp, float hum, long rssi, String uptime, bool relayState);

#endif