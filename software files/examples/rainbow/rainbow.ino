#include <Arduino.h>
#include "T3A33BRG.h"

#define DATA_PIN   25
#define CLOCK_PIN  26
#define NUM_LEDS   24

T3A33BRG strip(DATA_PIN, CLOCK_PIN);
uint8_t hueOffset = 0;  // Hue offset value rotating between 0 and 255

/**
 * @brief wheel
 * @param pos 0-255
 * @param[out] r
 * @param[out] g
 * @param[out] b
 *
 * Generate RGB values from Hue value.
 * Hue (0-255) is converted to RGB (0-255) using a simple linear interpolation.
 * The resulting colors are:
 * - Red (0-85): increasing red, decreasing green, no blue
 * - Green (85-170): decreasing red, increasing green, no blue
 * - Blue (170-255): no red, decreasing green, increasing blue
 */
void wheel(uint8_t pos, uint8_t &r, uint8_t &g, uint8_t &b) {
  if (pos < 85) {
    r = pos * 3;
    g = 255 - pos * 3;
    b = 0;
  } else if (pos < 170) {
    pos -= 85;
    r = 255 - pos * 3;
    g = 0;
    b = pos * 3;
  } else {
    pos -= 170;
    r = 0;
    g = pos * 3;
    b = 255 - pos * 3;
  }
}

void setup() {
  strip.begin();  
}

void loop() {
  // 1) Start Frame: 32 bit 0
  strip.send32Bit(0, 0, 0, 0, 0);

  // 2) LED Frames: color data for each LED
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    // LED position plus hueOffset, wrapped to 0–255
    uint8_t pos = (i * 256 / NUM_LEDS + hueOffset) & 0xFF;
    uint8_t r, g, b;
    wheel(pos, r, g, b);
    // flag=0x07, dim=0x1F (max brightness), B,G,R order
    strip.send32Bit(0x07, 0x02, b, g, r);
  }

  // 4) Move to the next step
  hueOffset++;
  delay(1);
}
