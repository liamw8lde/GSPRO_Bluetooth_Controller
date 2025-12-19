#include "BleCombo.h"

BleCombo::BleCombo(std::string name) : _bleKeyboard(name, "Espressif", 100) {
}

void BleCombo::begin() {
    _bleKeyboard.begin();
}

bool BleCombo::isConnected() {
    return _bleKeyboard.isConnected();
}

void BleCombo::k_press(uint8_t k) {
    _bleKeyboard.press(k);
}

void BleCombo::k_release(uint8_t k) {
    _bleKeyboard.release(k);
}

void BleCombo::k_releaseAll() {
    _bleKeyboard.releaseAll();
}

void BleCombo::k_write(uint8_t k) {
    _bleKeyboard.write(k);
}

void BleCombo::m_click(uint8_t b) {
    _bleKeyboard.click(b);
}

void BleCombo::m_press(uint8_t b) {
    _bleKeyboard.press(b);
}

void BleCombo::m_release(uint8_t b) {
    _bleKeyboard.release(b);
}

void BleCombo::m_move(int8_t x, int8_t y) {
    _bleKeyboard.move(x, y);
}
