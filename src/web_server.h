#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WebServer.h>

// Export the server object so main.cpp can use it in the loop
extern WebServer server;

// Function to setup routes, OTA, and start the server
void setupWebServer();

#endif