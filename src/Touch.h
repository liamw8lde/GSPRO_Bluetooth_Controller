#ifndef TOUCH_H
#define TOUCH_H

#include <Arduino.h>
#include <Wire.h>

class Touch {
public:
    Touch();
    void begin();
    bool getTouch(uint16_t *x, uint16_t *y);

private:
    static const uint8_t I2C_ADDR = 0x38;
};

#endif
