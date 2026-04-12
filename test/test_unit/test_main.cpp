#include <Arduino.h>
#include <Unity.h>
#include <Preferences.h>
#include <DHT.h>
#include "wifi_config.h"

// Declare external uptime function
extern String getFormattedUptime();

void setUp(void) {}
void tearDown(void) {}

// --- TEST 1: Uptime logic ---
void test_uptime_format(void) {
    String uptime = getFormattedUptime(); 
    TEST_ASSERT_EQUAL(8, uptime.length()); 
    // Initial uptime should be very low
    String hours_mins = uptime.substring(0, 6);
    TEST_ASSERT_EQUAL_STRING("00:00:", hours_mins.c_str());
}

// --- TEST 2: Flash Memory ---
void test_flash_memory_preferences(void) {
    Preferences prefs;
    prefs.begin("test_mem", false);
    prefs.putString("test_key", "AuraHub_OK");
    String readBack = prefs.getString("test_key", "FAIL");
    prefs.end();
    TEST_ASSERT_EQUAL_STRING("AuraHub_OK", readBack.c_str());
}

// --- TEST 3: Physical LED Toggle ---
void test_hardware_led_state(void) {
    // Seeed XIAO ESP32S3 user LED is usually Pin 44
    const int testLed = 44; 
    pinMode(testLed, OUTPUT);
    
    digitalWrite(testLed, HIGH);
    delay(100); // Small stabilization delay
    TEST_ASSERT_EQUAL(HIGH, digitalRead(testLed));
    
    digitalWrite(testLed, LOW);
    delay(100);
    TEST_ASSERT_EQUAL(LOW, digitalRead(testLed));
}

// --- TEST 4: Wi-Fi Rejection Logic ---
void test_wifi_failure_handling(void) {
    // We expect this to return 'false' because credentials are fake
    // Using a 5-second timeout for the test to remain snappy
    bool connected = connectSTA("NON_EXISTENT_SSID", "WRONG_PASS", 5000);
    TEST_ASSERT_FALSE(connected);
}

// --- TEST 5: DHT11 Sensor Integrity ---
void test_dht11_sensor_readings(void) {
    DHT test_dht(5, DHT11); 
    test_dht.begin();
    
    // DHT11 needs at least 2 seconds between readings
    delay(2500); 

    float t = test_dht.readTemperature();
    float h = test_dht.readHumidity();

    // Verification
    TEST_ASSERT_MESSAGE(!isnan(t), "Temperature reading is NaN (Check Wiring!)");
    TEST_ASSERT_MESSAGE(!isnan(h), "Humidity reading is NaN (Check Wiring!)");

    // Physical boundaries check
    TEST_ASSERT_TRUE(t > -20.0 && t < 70.0);
    TEST_ASSERT_TRUE(h >= 0.0 && h <= 100.0);
}

void setup() {
    // Extended delay to ensure USB Serial is ready on your PC
    delay(5000); 
    
    UNITY_BEGIN(); 
    
    // Running tests
    RUN_TEST(test_uptime_format);
    RUN_TEST(test_flash_memory_preferences);
    RUN_TEST(test_hardware_led_state);
    RUN_TEST(test_wifi_failure_handling);
    RUN_TEST(test_dht11_sensor_readings);
    
    UNITY_END(); 
}

void loop() {}