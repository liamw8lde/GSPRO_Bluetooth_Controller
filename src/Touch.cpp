#include "Touch.h"

Touch::Touch() {}

void Touch::begin() {
    Wire.begin(18, 19); // SDA, SCL for WT32-SC01
    Wire.setClock(400000); // 400kHz for faster I2C
    Serial.println("Touch I2C initialized on SDA:18, SCL:19");
}

bool Touch::getTouch(uint16_t *x, uint16_t *y) {
    // Try to read status register
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(0x02); 
    if (Wire.endTransmission() != 0) return false;

    if (Wire.requestFrom(I2C_ADDR, (uint8_t)1) != 1) return false;
    uint8_t touchPoints = Wire.read();

    if (touchPoints == 0 || touchPoints > 2) return false;

    // Read coordinates
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(0x03); 
    if (Wire.endTransmission() != 0) return false;

    if (Wire.requestFrom(I2C_ADDR, (uint8_t)4) != 4) return false;

    uint8_t x_high = Wire.read();
    uint8_t x_low = Wire.read();
    uint8_t y_high = Wire.read();
    uint8_t y_low = Wire.read();

    *x = ((x_high & 0x0F) << 8) | x_low;
    *y = ((y_high & 0x0F) << 8) | y_low;
    
    // Serial.printf("Raw Touch: X=%d, Y=%d\n", *x, *y); // Commented out for performance

    return true;
}
