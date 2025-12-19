#ifndef BLE_COMBO_H
#define BLE_COMBO_H

#include <BleKeyboard.h>

// Keyboard Modifiers (Map to BleKeyboard constants)
#define KEY_LEFT_CTRL   0x80
#define KEY_LEFT_SHIFT  0x81
#define KEY_LEFT_ALT    0x82
#define KEY_LEFT_GUI    0x83
#define KEY_RIGHT_CTRL  0x84
#define KEY_RIGHT_SHIFT 0x85
#define KEY_RIGHT_ALT   0x86
#define KEY_RIGHT_GUI   0x87

// Keyboard Keys (Standard HID - BleKeyboard handles mapping)
#define KEY_UP_ARROW    0xDA
#define KEY_DOWN_ARROW  0xD9
#define KEY_LEFT_ARROW  0xD8
#define KEY_RIGHT_ARROW 0xD7
#define KEY_F5          0xC6

class BleCombo {
public:
    BleCombo(std::string name = "GSPRO Controller");
    void begin();
    bool isConnected();
    
    // Keyboard
    void k_press(uint8_t k);
    void k_release(uint8_t k);
    void k_write(uint8_t k);
    void k_releaseAll();

    // Mouse (Stubs to keep main.cpp happy)
    void m_click(uint8_t b) {}
    void m_press(uint8_t b) {}
    void m_release(uint8_t b) {}
    void m_move(int8_t x, int8_t y) {}

private:
    BleKeyboard _bleKeyboard;
};

#endif