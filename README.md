# GSPRO Controller

A custom ESP32-based wireless touchscreen controller for the GSPRO Golf Simulator. This project turns a WT32-SC01 ESP32 touch display into a dedicated WiFi controller with full keyboard and mouse support for GSPRO.

![Version](https://img.shields.io/badge/Version-1.0.0-blue)
![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![WiFi](https://img.shields.io/badge/WiFi-UDP-orange)
![LVGL](https://img.shields.io/badge/UI-LVGL%208.3-purple)

## 🚀 Features

- **WiFi Connectivity:** Fast and reliable UDP communication (no Bluetooth driver issues!)
- **Full Keyboard & Mouse Support:**
  - All GSPRO keyboard shortcuts
  - Touchpad mode with mouse movement
  - Left and right click buttons
- **Dedicated GSPRO Controls:**
  - **Game Actions:** Mulligan (Ctrl+M), Pin, Scout
  - **Views:** Heat Map, Flyover, Free Camera (F5)
  - **Navigation:** Arrow keys for aiming and Tee adjustments
- **Modern Touchscreen UI:** High-contrast interface built with LVGL 8.3
- **Windows Compatible:** No driver issues, works perfectly on Windows 10/11
- **Easy Setup:** Simple WiFi configuration and Python receiver app
- **System Tray Integration:** Runs in background with auto-start capability

## 🛠 Hardware

- **Board:** WT32-SC01 (ESP32-WROVER-B)
- **Display:** 3.5" 480x320 Capacitive Touch Screen
- **CPU:** Dual-core ESP32, 240MHz
- **Memory:** 520KB SRAM + 4MB PSRAM
- **Connectivity:** WiFi 2.4GHz (802.11 b/g/n)
- **Power:** 5V USB-C

**See [HARDWARE.md](HARDWARE.md) for detailed specifications, purchasing guide, and alternative hardware options.**

## 🔧 Quick Start

**See [QUICK_START.md](QUICK_START.md) for the complete step-by-step guide!**

### 1. Hardware Setup
- Purchase a WT32-SC01 board (~$15-25)
- Connect via USB-C to your computer

### 2. Configure WiFi
Edit `src/config.h` with your WiFi credentials:
```cpp
#define WIFI_SSID "YOUR_WIFI_NETWORK"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define PC_IP_ADDRESS "192.168.1.100"  // Your PC's IP
```

### 3. Upload Firmware
Using Visual Studio Code + PlatformIO:
```bash
git clone https://github.com/liamw8lde/GSPRO_Bluetooth_Controller.git
cd GSPRO_Bluetooth_Controller
# Open in VS Code, then click Upload button
```

### 4. Install Python Receiver (PC)
```bash
pip install -r requirements.txt
python gspro_receiver_tray.py  # System tray version
```

### 5. Start Using!
Power on the controller, wait for WiFi connection, and start playing!

---

## 📚 Documentation

- **[QUICK_START.md](QUICK_START.md)** - Fast setup guide
- **[WIFI_SETUP.md](WIFI_SETUP.md)** - Detailed WiFi configuration
- **[TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - Common issues and solutions
- **[API_REFERENCE.md](API_REFERENCE.md)** - UDP protocol documentation
- **[HARDWARE.md](HARDWARE.md)** - Hardware specs and purchasing guide
- **[CONTRIBUTING.md](CONTRIBUTING.md)** - How to contribute
- **[CHANGELOG.md](CHANGELOG.md)** - Version history

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

## 🎯 Why WiFi Instead of Bluetooth?

Version 1.0.0 switched from Bluetooth to WiFi UDP for better reliability:

✅ **Mouse Support** - Bluetooth HID mouse had driver issues on Windows
✅ **No Pairing** - No Bluetooth pairing hassles
✅ **Better Range** - WiFi works through walls
✅ **Lower Latency** - UDP is faster than Bluetooth HID
✅ **No Driver Errors** - Eliminates Windows "Driver Error" problems
✅ **More Reliable** - Stable connection, no random disconnects

## 🔧 Advanced Features

### System Tray Application

Run `gspro_receiver_tray.py` for:
- Background operation
- System tray icon
- Auto-start with Windows
- Visual connection status

### Build Standalone Executable

Create a portable .exe file:
```bash
build_exe.bat  # Windows only
```

This creates a standalone receiver that doesn't require Python installation.

### Protocol Documentation

The controller uses a simple UDP protocol. See [API_REFERENCE.md](API_REFERENCE.md) to:
- Understand the communication protocol
- Build your own receiver
- Create custom integrations
- Develop companion apps

## 🤝 Contributing

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for:
- How to contribute code
- Coding standards
- Pull request process
- Development setup

Quick contribution ideas:
- Add new button layouts
- Create alternative themes
- Improve documentation
- Port to other ESP32 boards
- Add new features

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

You are free to:
- Use commercially
- Modify and distribute
- Use privately
- Sublicense

## 🙏 Acknowledgments

Built with these excellent open-source projects:
- [PlatformIO](https://platformio.org/) - Development platform
- [LVGL](https://lvgl.io/) - Graphics library
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) - Display driver
- [pynput](https://github.com/moses-palmer/pynput) - Python input control

## ⭐ Support

If you find this project useful:
- Star the repository ⭐
- Share with the GSPRO community
- Report bugs and suggest features
- Contribute improvements

## 📞 Contact & Support

- **Issues**: [GitHub Issues](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/issues)
- **Discussions**: [GitHub Discussions](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/discussions)
- **Questions**: Open a discussion or issue

## 🚀 Roadmap

Future enhancements planned:
- [ ] Web-based configuration interface
- [ ] OTA (Over-The-Air) firmware updates
- [ ] Multiple controller support
- [ ] Custom button mapping via UI
- [ ] Battery level monitoring
- [ ] Android/iOS companion app
- [ ] Multiple GSPRO profiles

See [CHANGELOG.md](CHANGELOG.md) for version history and [CONTRIBUTING.md](CONTRIBUTING.md) to help make these happen!

---

**Made with ❤️ for the GSPRO Golf Simulator community**
