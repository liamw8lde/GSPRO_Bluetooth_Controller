# GSPRO Bluetooth Controller

A custom ESP32-based touch screen controller for the GSPRO Golf Simulator. This project turns a WT32-SC01 (or similar ESP32 touch display) into a wireless BLE Keyboard dedicated to GSPRO shortcuts.

![Project Status](https://img.shields.io/badge/Status-Fixed-brightgreen)
![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Library](https://img.shields.io/badge/Library-LVGL%20%7C%20BLE--Keyboard-orange)

## 🚀 Features

- **Wireless Connectivity:** Connects via Bluetooth Low Energy (BLE).
- **Dedicated GSPRO Controls:**
  - **Game Actions:** Mulligan (Ctrl+M), Pin, Scout.
  - **Views:** Heat Map, Flyover, Free Camera (F5).
  - **Navigation:** Arrow keys for aiming and Tee adjustments.
- **Custom UI:** Modern high-contrast interface built with LVGL 8.3.
- **Windows Optimized:** Includes specific fixes for common "Driver Error" issues on Windows 10/11.

## 🛠 Hardware

- **Controller:** ESP32 (Dual Core, 240MHz).
- **Display:** 3.5" 480x320 Capacitive Touch Screen (WT32-SC01).
- **Backlight Control:** Integrated PWM control.

## 🔧 Installation

Built using **PlatformIO**.

1. Clone this repository.
2. Install the following libraries (automatically handled by PlatformIO):
   - `T-vK/ESP32 BLE Keyboard`
   - `lvgl/lvgl`
   - `bodmer/TFT_eSPI`
   - `h2zero/NimBLE-Arduino`
3. Connect your ESP32 and run:
   ```bash
   platformio run --target upload
   ```

## 📋 Windows "Driver Error" Fixes

If you encounter a "Driver Error" on Windows, this project includes the following built-in mitigations:
- **Standard Library:** Switched to the industry-standard `ESP32-BLE-Keyboard` for maximum HID compatibility.
- **MAC Rotation:** The `setup()` function includes logic to rotate the device's MAC address, forcing Windows to treat it as a brand new device and bypass corrupt driver caches.
- **Connection Parameters:** Updated BLE connection intervals (~20-40ms) to meet Windows HID stability requirements.
- **PnP ID Spoofing:** Identifies as a standard HID Keyboard to ensure the default Windows `kbdhid.sys` driver is used.

## ⌨️ Key Mappings

| UI Button | Key / Shortcut |
|-----------|----------------|
| Mulligan  | Ctrl + M       |
| Pin       | P              |
| Scout     | J              |
| Heat      | Z (Y)          |
| Flyover   | O              |
| Free Cam  | F5             |
| Arrows    | Up/Down/LT/RT  |

## 🤝 Contributing
Feel free to fork and add your own custom buttons or layout styles!
