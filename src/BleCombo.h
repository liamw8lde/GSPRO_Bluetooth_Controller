#ifndef WIFI_COMBO_H
#define WIFI_COMBO_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string>

// Keyboard Modifiers
#define KEY_LEFT_CTRL   0x80
#define KEY_LEFT_SHIFT  0x81
#define KEY_LEFT_ALT    0x82
#define KEY_LEFT_GUI    0x83
#define KEY_RIGHT_CTRL  0x84
#define KEY_RIGHT_SHIFT 0x85
#define KEY_RIGHT_ALT   0x86
#define KEY_RIGHT_GUI   0x87

// Keyboard Keys
#define KEY_UP_ARROW    0xDA
#define KEY_DOWN_ARROW  0xD9
#define KEY_LEFT_ARROW  0xD8
#define KEY_RIGHT_ARROW 0xD7
#define KEY_F5          0xC6

// Mouse Buttons
#define MOUSE_LEFT      0x01
#define MOUSE_RIGHT     0x02
#define MOUSE_MIDDLE    0x04

class BleComboWrapper {
public:
    BleComboWrapper(std::string name = "GSPRO Controller");
    void begin();
    bool isConnected();

    // Keyboard
    void k_press(uint8_t k);
    void k_release(uint8_t k);
    void k_write(uint8_t k);
    void k_releaseAll();

    // Mouse
    void m_click(uint8_t b);
    void m_press(uint8_t b);
    void m_release(uint8_t b);
    void m_move(int8_t x, int8_t y);

private:
    std::string _deviceName;
    WiFiUDP _udp;
    IPAddress _pcIP;
    uint16_t _pcPort;
    bool _wifiConnected;

    void sendCommand(uint8_t cmd, uint8_t* data, size_t len);
};

#endif