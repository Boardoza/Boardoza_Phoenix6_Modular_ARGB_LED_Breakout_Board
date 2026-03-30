#include "T3A33BRG.h"

/**
 * @brief Constructor for the T3A33BRG class.
 *
 * Initializes the T3A33BRG object by assigning the provided data and clock pins
 * to the internal variables `_dataPin` and `_clockPin`. These pins are later used
 * for communication with the LED controller.
 *
 * @param dataPin The pin number used for data transmission to the LED controller.
 * @param clockPin The pin number used for clock signal transmission to the LED controller.
 *
 * Example usage:
 * @code
 * T3A33BRG ledController(25, 26);  // Create an instance with data pin 25 and clock pin 26
 * @endcode
 */
T3A33BRG::T3A33BRG(uint8_t dataPin, uint8_t clockPin) : _dataPin(dataPin), _clockPin(clockPin) {}

/**
 * @brief Initializes the T3A33BRG LED controller.
 *
 * This function sets up the data and clock pins as outputs and ensures
 * that their initial states are set to LOW. It should be called before
 * using any other functions of the T3A33BRG class.
 * 
 * Example usage:
 * @code
 * ledController.begin();  // Initialize the controller
 * @endcode
 */
void T3A33BRG::begin() {
    pinMode(_dataPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);

    digitalWrite(_dataPin, LOW);
    digitalWrite(_clockPin, LOW);
}

/**
 * @brief Sends a 32-bit data frame to the LED controller.
 *
 * This function constructs a 32-bit data frame using the provided flag, dimming,
 * and RGB color values, then sends it bit by bit over the data and clock pins.
 * It is used to control the brightness and color of LEDs.
 *
 * @param flag    3-bit flag (bits 31 to 29) that defines specific settings for the LED.
 * @param dimming 5-bit dimming value (bits 28 to 24) that adjusts the brightness.
 * @param blue    8-bit blue color intensity (bits 23 to 16).
 * @param green   8-bit green color intensity (bits 15 to 8).
 * @param red     8-bit red color intensity (bits 7 to 0).
 *
 * Example usage:
 * @code
 * ledController.send32Bit(0x07, 0x1F, 255, 128, 64);  // Send a 32-bit frame
 * @endcode
 */
void T3A33BRG::send32Bit(uint32_t flag, uint32_t dimming, uint32_t blue, uint32_t green, uint32_t red) {
    uint32_t data = 0;

     // Flag (3 bit) [31:29]
    data |= ((uint32_t)(flag & 0x07)) << 29;

    // Dimming (5 bit) [28:24]
    data |= ((uint32_t)(dimming & 0x1F)) << 24;

    // Blue (8 bit) [23:16]
    data |= ((uint32_t)(blue & 0xFF)) << 16;

    // Green (8 bit) [15:8]
    data |= ((uint32_t)(green & 0xFF)) << 8;

    // Red (8 bit) [7:0]
    data |= ((uint32_t)(red & 0xFF));

    for(int i = 31; i >= 0; i--) {
        digitalWrite(_dataPin, (data & (1UL << i)) ? HIGH : LOW);
        clockPulse();
    }
    
}

/**
 * @brief Generates a clock pulse on the clock pin.
 *
 * This function sets the clock pin (`_clockPin`) HIGH for 1 microsecond,
 * then LOW for another 1 microsecond to create a single clock pulse. It is used
 * to synchronize data transmission with the LED controller.
 * 
 * Example usage:
 * @code
 * ledController.clockPulse();  // Generate a single clock pulse
 * @endcode
 */
void T3A33BRG::clockPulse() {
    digitalWrite(_clockPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(_clockPin, LOW);
    delayMicroseconds(1);
}