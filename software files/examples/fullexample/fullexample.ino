#include "EC1101S.h"
#include "T3A33BRG.h"

// Rotary Encoder Pins
#define TERMINAL_A 33
#define TERMINAL_B 21
#define SW         22

// LED Control Pins
#define DATA_PIN   25  // Suitable output pin
#define CLOCK_PIN  26  // Suitable output pin

// Create objects
EC1101S encoder(TERMINAL_A, TERMINAL_B, SW);
T3A33BRG ledController(DATA_PIN, CLOCK_PIN);

// Maximum number of LEDs
const uint8_t LED_COUNT = 12; // Total number of LEDs
uint8_t currentLED = 0;      // Index of the LED that is lit
uint8_t turnCounter = 0;     // Counter for 5 rotations
int8_t lastCount=0;
int8_t currentCount=0; 

/**
 * @brief Lights up a specific LED and turns off the rest.
 *
 * This function lights up the LED at the specified index while turning off
 * all the other LEDs. The `ledIndex` is wrapped around the maximum LED count (12)
 * using the modulo operation. 
 *
 * @param ledIndex The index of the LED to light up (0-based).
 *
 * The function sends the following commands:
 * - A reset command to turn off all LEDs.
 * - Commands to light up the specific LED and turn off others.
 * - A final command to ensure all LEDs are turned off at the end of the operation.
 *
 * Example usage:
 * @code
 * lightUpLED(5);  // Lights up the 5th LED.
 * @endcode
 */
void lightUpLED(uint8_t ledIndex) {
    uint8_t mode = ledIndex % 12;
    ledController.send32Bit(0, 0, 0, 0, 0);
    for (uint8_t i = 0; i <= LED_COUNT; i++) {
        if (i == mode) {
                ledController.send32Bit(0x07, 1, 0, 0, 255); // Turn on the LED
        } 
        else {
            ledController.send32Bit(0x07, 0, 0, 0, 0); // Turn off
        }
    }
   ledController.send32Bit(0x07, 31, 255, 255, 255); // Turn off   
}

/**
 * @brief Turns on LEDs with random colors and optionally turns them off.
 *
 * This function lights up LEDs with random colors for a total of 12 iterations.
 * The `order` parameter determines the modulo value to calculate the mode.
 * The `on_off` parameter is used to control the state, but its functionality is not fully implemented in the current version.
 *
 * @param order Determines the mode based on modulo 12 operation.
 * @param on_off Not fully implemented, reserved for controlling on/off state.
 *
 * The function performs the following steps:
 * - Sends an initial reset command to turn off all LEDs.
 * - Iterates through 12 LEDs, turning each one on with a random RGB value.
 * - Sends a final command to turn off all LEDs.
 *
 * Example usage:
 * @code
 * randomLEDOn(5, 1);  // Lights up LEDs with random colors.
 * @endcode
 */                                                                                                                                                                                                                                                                                                                                                                          
void randomLEDOn(uint8_t order, uint8_t on_off)
{
uint8_t i=0; 
uint8_t mode=order%12;
ledController.send32Bit(0, 0, 0, 0, 0); 
while (i<=12)
    {
    i++; 
    ledController.send32Bit(0x07, 1, (rand()%255), (rand()%255), (rand()%255));          
    }
ledController.send32Bit(0x07, 31, 255, 255, 255); // Turn off
}

void setup() {
    Serial.begin(115200);

    // Initialize the encoder and LED controller
    encoder.begin();
    ledController.begin();

    // Reset all LEDs initially
    ledController.send32Bit(0, 0, 0, 0, 0);
}

void loop() {

    encoder.update();

    currentCount = encoder.getCount();

    if (currentCount != lastCount) {
        //randomLEDOn(currentCount, 1);     // Uncomment this line if you want to use the randomLEDOn function
        // Determine direction
        if (currentCount > lastCount) {
            turnCounter++; // Clockwise -> Forward
            //if (turnCounter >= 1 && currentLED < LED_COUNT) { // use this block if you want to light all leds at the same time
            if (turnCounter >= 1 ) {                            // use this block if you want to light leds one by one
                currentLED++; // Light up the next LED
                turnCounter = 0; // Reset the counter
            }
        } else {
            turnCounter++; // Counter-clockwise -> Backward
            if (turnCounter >= 1 && currentLED > 0) {
                currentLED--; // Turn off the previous LED
                turnCounter = 0; // Reset the counter
            }
        }
    Serial.println(currentCount);

    // Change the LED
    lightUpLED(currentLED);     // Comment this line if you want to use the randomLEDOn function
    lastCount = currentCount;
    }
}
