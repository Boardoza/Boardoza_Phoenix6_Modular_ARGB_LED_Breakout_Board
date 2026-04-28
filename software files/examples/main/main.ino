#include <Arduino.h>
#include "T3A33BRG.h"

// LED Control Pins
#define DATA_PIN  25
#define CLOCK_PIN 26

// T3A33BRG Object
T3A33BRG ledController(DATA_PIN, CLOCK_PIN);

void setup() {
    Serial.begin(115200);

    // Initialize the LED control module
    ledController.begin();

    // Reset all LEDs initially
    ledController.send32Bit(0, 0, 0, 0, 0);
    Serial.println("12 pieces of 32-bit data sent.");
}

void loop() {
    // Turn on 12 LEDs sequentially
    for (uint8_t i = 0; i < 12; i++) {
        ledController.send32Bit(7, 0, 0, 0, 255);  // Blue colored LED
        delay(1000); // Wait 1 second
    }
    ledController.send32Bit(0, 0, 0, 0, 0);
    // Turn off 12 LEDs sequentially
    for (uint8_t i = 0; i < 14; i++) {
        ledController.send32Bit(7, 0, 0, 0, 0);  // LED off
        delay(1000); // Wait 1 second
    }
    ledController.send32Bit(0, 0, 0, 0, 0);
}
