#ifndef LIVE_DATA_H
#define LIVE_DATA_H

#include <Arduino.h>
#include <WebSocketsServer.h>

// Exporting the WebSocket object so other files can use it if needed
extern WebSocketsServer webSocket;

// Function declarations
void setupWebSockets();
void handleLiveData();
void broadcastLog(const String& msg);
void blinkConfirm();

#endif