#include <Arduino.h>
#include "T3A33BRG.h"

#define DATA_PIN   25
#define CLOCK_PIN  26
#define NUM_LEDS   12

T3A33BRG strip(DATA_PIN, CLOCK_PIN);
uint8_t hueOffset = 0;  // 0…255 arası dönen kaydırma değeri

void startFrame() {
  strip.send32Bit(0, 0, 0, 0, 0);
}

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

/**
 * @brief Fades in and out the LEDs
 *
 * This function fades in the LEDs from minimum brightness to maximum brightness and then fades out from maximum brightness to minimum brightness.
 * The fade in and fade out are done in 31 steps with 50ms delay between each step.
 */
void fadeInOut() {
  // Fade In
  for (uint8_t d = 0; d <= 31; d++) {
    startFrame();
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      strip.send32Bit(0x07, d, 255, 255, 255);
    }
    delay(50);
  }
  // Fade Out
  for (int8_t d = 31; d >= 0; d--) {
    startFrame();
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      strip.send32Bit(0x07, d, 255, 255, 255);
    }
    delay(50);
  }
}

void setup() {
  strip.begin();  
}

void loop() {
  fadeInOut();
}