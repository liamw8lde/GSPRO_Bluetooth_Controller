# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-12-20

### Major Release - WiFi UDP Controller

This is the first stable release of the GSPRO Controller, featuring a complete rewrite from Bluetooth to WiFi UDP for better Windows compatibility.

### Added
- WiFi UDP communication for keyboard and mouse control
- Touchpad screen with mouse control functionality
- Python receiver application (`gspro_receiver.py`) for PC
- System tray application (`gspro_receiver_tray.py`) with auto-start capability
- Executable builder script (`build_exe.bat`) for creating standalone Windows application
- Comprehensive documentation:
  - `QUICK_START.md` - Quick start guide
  - `WIFI_SETUP.md` - Detailed WiFi setup instructions
  - `CHANGELOG.md` - Version history
  - `CONTRIBUTING.md` - Contribution guidelines
  - `TROUBLESHOOTING.md` - Troubleshooting guide
  - `API_REFERENCE.md` - UDP protocol documentation
  - `HARDWARE.md` - Hardware specifications
- VS Code/PlatformIO setup instructions in README
- WiFi configuration file (`src/config.h`)
- Mouse movement and click support
- Touchpad UI mode with left/right click buttons
- WiFi connection status display on controller

### Changed
- **BREAKING**: Switched from Bluetooth HID to WiFi UDP protocol
- Updated README with WiFi setup instructions
- Improved UI with WiFi status indicator
- Changed UDP port from 5005 to 5006 to avoid Windows Media Player conflicts
- Enhanced error handling in Python receiver

### Fixed
- Windows Bluetooth "Driver Error" issues (eliminated by switching to WiFi)
- Mouse functionality on Windows (now works perfectly via WiFi)
- Connection reliability issues
- Latency improvements over Bluetooth

### Technical Details
- Protocol: WiFi UDP (port 5006)
- Python dependencies: pynput, pystray, Pillow, pyinstaller
- ESP32 libraries: TFT_eSPI, LVGL 8.3, Wire
- Display: 480x320 landscape orientation
- Memory: PSRAM enabled for LVGL buffers

## [0.3.0] - 2025-12-18

### Added
- Touchpad screen with mouse control functionality
- Mouse movement tracking via touch
- Left and right click buttons
- Pull request #1 merged

## [0.2.0] - 2025-12-17

### Added
- Visual Studio Code setup instructions to README
- Detailed installation and setup instructions
- Comprehensive README with badges and feature list

### Improved
- Documentation structure
- Setup process clarity

## [0.1.0] - 2025-12-16

### Initial Release - Bluetooth Version

- ESP32-based BLE Keyboard controller for GSPRO Golf Simulator
- WT32-SC01 touch display support (3.5" 480x320)
- LVGL 8.3 UI framework
- Custom high-contrast dark theme
- Bluetooth Low Energy (HID Keyboard) connectivity
- Key mappings for GSPRO shortcuts:
  - Mulligan (Ctrl+M)
  - Pin (P)
  - Scout (J)
  - Heat Map (Z)
  - Flyover (O)
  - Free Camera (F5)
  - Arrow keys for navigation
  - Tee position adjustment (C/V)
- Hold-to-repeat functionality for navigation keys
- PlatformIO build configuration
- Initial project structure

### Known Issues (Resolved in v1.0.0)
- Bluetooth driver errors on Windows 10/11
- Mouse functionality not supported via Bluetooth HID
- Connection reliability issues
- Pairing problems on some Windows systems

---

## Version History Summary

- **v1.0.0** (2025-12-20) - WiFi UDP implementation, mouse support, stable release
- **v0.3.0** (2025-12-18) - Added touchpad/mouse functionality (Bluetooth)
- **v0.2.0** (2025-12-17) - Documentation improvements
- **v0.1.0** (2025-12-16) - Initial Bluetooth implementation

## Upgrade Guide

### From v0.x.x (Bluetooth) to v1.0.0 (WiFi)

1. **Update ESP32 Code**: Pull latest code and upload to your ESP32
2. **Configure WiFi**: Edit `src/config.h` with your WiFi credentials and PC IP
3. **Install Python**: Install Python 3.8+ on your PC
4. **Install Dependencies**: Run `pip install -r requirements.txt`
5. **Run Receiver**: Start `gspro_receiver_tray.py` on your PC
6. **Remove Bluetooth Pairing**: Unpair the old "GSPRO Controller" Bluetooth device from Windows

## Future Roadmap

- [ ] Android/iOS companion app
- [ ] Web-based configuration interface
- [ ] Multiple controller support
- [ ] Custom button mapping via UI
- [ ] Firmware OTA updates
- [ ] Battery level monitoring (for portable builds)
- [ ] Multiple GSPRO profiles

## Links

- [GitHub Repository](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller)
- [Issue Tracker](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/issues)
- [Documentation](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/blob/main/README.md)
