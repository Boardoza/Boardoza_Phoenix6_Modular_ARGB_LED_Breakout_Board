#ifndef T3A33BRG_H
#define T3A33BRG_H

#include "Arduino.h"

class T3A33BRG {
public:
    T3A33BRG(uint8_t dataPin, uint8_t clockPin);
    void begin();
    void send32Bit(uint32_t flag, uint32_t dimming, uint32_t blue, uint32_t green, uint32_t red);
private:
    uint8_t _dataPin;
    uint8_t _clockPin;
    void clockPulse();
};

#endif