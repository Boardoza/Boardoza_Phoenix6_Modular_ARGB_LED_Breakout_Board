#include <Arduino.h>
#include "T3A33BRG.h"

// LED Kontrol Pinleri
#define DATA_PIN  25
#define CLOCK_PIN 26

// T3A33BRG Nesnesi
T3A33BRG ledController(DATA_PIN, CLOCK_PIN);

void setup() {
    Serial.begin(115200);

    // LED kontrol modülünü başlat
    ledController.begin();

    // Başlangıçta tüm LED'leri sıfırla
    ledController.send32Bit(0, 0, 0, 0, 0);
    Serial.println("12 adet 32-bit veri gönderildi.");
}

void loop() {
    // 12 LED'i sırayla yak
    for (uint8_t i = 0; i < 12; i++) {
        ledController.send32Bit(7, 0, 0, 0, 255);  // Mavi renkli LED
        delay(1000); // 1 saniye bekle
    }
    ledController.send32Bit(0, 0, 0, 0, 0);
    // 12 LED'i sırayla söndür
    for (uint8_t i = 0; i < 14; i++) {
        ledController.send32Bit(7, 0, 0, 0, 0);  // LED kapalı
        delay(1000); // 1 saniye bekle
    }
    ledController.send32Bit(0, 0, 0, 0, 0);
}
