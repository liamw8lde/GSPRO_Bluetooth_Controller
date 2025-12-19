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

## 🔧 Installation & Setup

### Option A: Visual Studio Code (Recommended)
1. Download and install [Visual Studio Code](https://code.visualstudio.com/).
2. Open VS Code and go to the **Extensions** view (Ctrl+Shift+X).
3. Search for and install the **PlatformIO IDE** extension.
4. Clone this repository or open the project folder in VS Code.
5. Wait for PlatformIO to initialize and download dependencies.
6. Connect your ESP32 via USB.
7. Click the **PlatformIO: Upload** button (arrow icon) in the bottom status bar.

### Option B: PlatformIO CLI
If you prefer the command line, you can install the Core CLI using PowerShell:

```powershell
# Download the installer
Invoke-WebRequest -Uri https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -OutFile get-platformio.py

# Run the installer
python get-platformio.py

# Add to PATH (standard path)
$env:PATH += ";$env:USERPROFILE\.platformio\penv\Scripts"
```

### 2. Clone and Build
```bash
# Clone the repo
git clone https://github.com/liamw8lde/GSPRO_Bluetooth_Controller.git
cd GSPRO_Bluetooth_Controller

# Build and Upload to your ESP32
# Replace 'pio' with the full path if not in your PATH yet
pio run --target upload
```

### 3. Windows Bluetooth Connection
1. Power on your ESP32 device.
2. Open **Windows Settings > Bluetooth & Devices**.
3. Click **Add Device**.
4. Select **GSPRO Controller** from the list.
5. Once paired, Windows will automatically install it as a standard HID Keyboard.

---

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
