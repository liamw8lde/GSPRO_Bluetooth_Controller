# Project Status: GSPRO Golf Controller (WT32-SC01)
**Date:** December 18, 2025
**Path:** C:\Project\Blue

## 1. Hardware Setup
*   **Board:** WT32-SC01 (ESP32-WROVER-B)
*   **Display:** 320x480 Landscape (ST7796 Driver)
*   **Touch:** Capacitive (FT6336U Driver)
*   **Protocol:** Bluetooth Low Energy (HID Keyboard)

## 2. Current Configuration
*   **Bluetooth Stack:** Standard `ESP32 BLE Keyboard` (NimBLE removed for stability).
*   **Memory Management:** Dynamic allocation for LVGL buffer (PSRAM enabled).
*   **Splash Screen:** Text-based "GS PRO" (Image splash removed to save resources).
*   **UI Theme:** High Contrast Dark Mode (Charcoal buttons, Neon Green borders, White Bold Text).

## 3. Key Mappings & Logic
| Button | Key | Type | Note |
| :--- | :--- | :--- | :--- |
| **Mulligan** | Ctrl + M | Click | |
| **Pin** | P | Click | |
| **Scout** | J | Click | |
| **Heat** | Z | Click | Sends 'z' for German layout compatibility (outputs 'y') |
| **Flyover** | O | Click | |
| **Free** | F5 | Click | |
| **Arrows** | Up/Dn/Lt/Rt| **Hold** | Repeats while held |
| **Tee L/R** | C / V | **Hold** | Repeats while held |

## 4. Recent Fixes
*   **Touch Responsiveness:** I2C clock increased to 400kHz.
*   **Visuals:** Fixed text color (forced white) and text clipping (shortened labels).
*   **Logic:** Added `repeat` flag to `KeyMap` struct to allow continuous scrolling for navigation keys.

## 5. Next Steps
*   Build and Upload using PlatformIO.
*   Pair with PC as "GSPRO Controller".
