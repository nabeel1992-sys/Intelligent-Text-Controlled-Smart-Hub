#include <Arduino.h>
#include <Unity.h>
#include <WebSocketsServer.h>
#include <DHT.h>           // DHT library include karein
#include "live_data.h"

// External references
extern void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
extern void sendSystemStatus();
extern float lastTemp;
extern float lastHum;
extern DHT dht;            // live_data.cpp wala dht object use karein

const int hardwarePin = 4;

void setUp(void) {
    // 1. Initialize DHT sensor (Very Important warna crash hoga)
    dht.begin(); 
    
    // 2. Setup pin state
    pinMode(hardwarePin, OUTPUT);
    digitalWrite(hardwarePin, LOW);
    
    // 3. Reset internal variables
    lastTemp = 0.0;
    lastHum = 0.0;
    
    delay(200); 
}

void tearDown(void) {
    digitalWrite(hardwarePin, LOW);
}

// ==========================================
// TEST 1: WebSocket Trigger logic
// ==========================================
void test_websocket_triggers_hardware(void) {
    TEST_ASSERT_EQUAL_INT(LOW, digitalRead(hardwarePin));

    uint8_t payload_on[] = "led_on";
    webSocketEvent(0, WStype_TEXT, payload_on, sizeof(payload_on) - 1);
    
    for(int i=0; i<10; i++) { delay(50); yield(); } // Increased stabilization
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(HIGH, digitalRead(hardwarePin), "LED failed to turn ON");

    uint8_t payload_off[] = "led_off";
    webSocketEvent(0, WStype_TEXT, payload_off, sizeof(payload_off) - 1);
    
    for(int i=0; i<10; i++) { delay(50); yield(); }
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(LOW, digitalRead(hardwarePin), "LED failed to turn OFF");
}

// ==========================================
// TEST 2: Sensor Data Processing
// ==========================================
void test_sensor_network_integration(void) {
    // DHT11 needs time between reads
    delay(2500);

    sendSystemStatus();

    // Check if readings are numeric
    TEST_ASSERT_FALSE_MESSAGE(isnan(lastTemp), "Temperature reading is NaN");
    TEST_ASSERT_FALSE_MESSAGE(isnan(lastHum), "Humidity reading is NaN");

    // Values should be updated from 0.0
    TEST_ASSERT_TRUE_MESSAGE(lastTemp > 0.0 || lastHum > 0.0, "Sensor data still at zero");
}

void setup() {
    // Wait longer for USB Serial to stabilize
    delay(5000); 
    Serial.begin(115200);
    while(!Serial); 

    Serial.println("--- STARTING INTEGRATION TESTS ---");
    
    UNITY_BEGIN(); 
    RUN_TEST(test_websocket_triggers_hardware);
    RUN_TEST(test_sensor_network_integration); 
    UNITY_END(); 
}

void loop() {}
