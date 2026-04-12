#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Arduino.h>
#include <WiFi.h>

// This variable will indicate whether we are in Hotspot mode or not.
extern bool apMode;

// Functions names
bool connectSTA(const String& ssid, const String& pass, uint32_t timeoutMs = 15000);
void startAP();

#endif