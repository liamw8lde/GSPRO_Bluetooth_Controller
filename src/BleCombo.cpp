#include "BleCombo.h"
#include "config.h"

// Command types
#define CMD_KEY_PRESS    1
#define CMD_KEY_RELEASE  2
#define CMD_KEY_WRITE    3
#define CMD_MOUSE_MOVE   4
#define CMD_MOUSE_CLICK  5
#define CMD_MOUSE_PRESS  6
#define CMD_MOUSE_RELEASE 7

BleComboWrapper::BleComboWrapper(std::string name) : _deviceName(name), _wifiConnected(false), _pcPort(UDP_PORT) {
    _pcIP.fromString(PC_IP_ADDRESS);
}

void BleComboWrapper::begin() {
    Serial.printf("Connecting to WiFi: %s\n", WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        _wifiConnected = true;
        Serial.printf("\nWiFi Connected! IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("Sending to PC: %s:%d\n", _pcIP.toString().c_str(), _pcPort);
        _udp.begin(0);  // Use any available port for sending
    } else {
        Serial.println("\nWiFi connection failed!");
        _wifiConnected = false;
    }
}

bool BleComboWrapper::isConnected() {
    return _wifiConnected && WiFi.status() == WL_CONNECTED;
}

void BleComboWrapper::sendCommand(uint8_t cmd, uint8_t* data, size_t len) {
    if (!isConnected()) return;

    uint8_t packet[32];
    packet[0] = cmd;
    memcpy(packet + 1, data, len);

    _udp.beginPacket(_pcIP, _pcPort);
    _udp.write(packet, len + 1);
    _udp.endPacket();
}

void BleComboWrapper::k_press(uint8_t k) {
    sendCommand(CMD_KEY_PRESS, &k, 1);
}

void BleComboWrapper::k_release(uint8_t k) {
    sendCommand(CMD_KEY_RELEASE, &k, 1);
}

void BleComboWrapper::k_releaseAll() {
    // Release all common keys
    uint8_t keys[] = {0x80, 0x81, 0x82}; // Ctrl, Shift, Alt
    for (uint8_t k : keys) {
        k_release(k);
    }
}

void BleComboWrapper::k_write(uint8_t k) {
    sendCommand(CMD_KEY_WRITE, &k, 1);
}

void BleComboWrapper::m_click(uint8_t b) {
    Serial.printf("Mouse click: %d\n", b);
    sendCommand(CMD_MOUSE_CLICK, &b, 1);
}

void BleComboWrapper::m_press(uint8_t b) {
    Serial.printf("Mouse press: %d\n", b);
    sendCommand(CMD_MOUSE_PRESS, &b, 1);
}

void BleComboWrapper::m_release(uint8_t b) {
    Serial.printf("Mouse release: %d\n", b);
    sendCommand(CMD_MOUSE_RELEASE, &b, 1);
}

void BleComboWrapper::m_move(int8_t x, int8_t y) {
    Serial.printf("Mouse move: x=%d, y=%d\n", x, y);
    uint8_t data[2] = {(uint8_t)x, (uint8_t)y};
    sendCommand(CMD_MOUSE_MOVE, data, 2);
}
