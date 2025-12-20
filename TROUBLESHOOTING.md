# Troubleshooting Guide

This guide helps you resolve common issues with the GSPRO Controller.

## Table of Contents

- [WiFi Connection Issues](#wifi-connection-issues)
- [ESP32 Upload Issues](#esp32-upload-issues)
- [Python Receiver Issues](#python-receiver-issues)
- [Display Issues](#display-issues)
- [Touch Screen Issues](#touch-screen-issues)
- [Performance Issues](#performance-issues)
- [Hardware Issues](#hardware-issues)
- [Advanced Debugging](#advanced-debugging)

## WiFi Connection Issues

### Controller Won't Connect to WiFi

**Symptoms**: Display shows "WiFi: Disconnected" or "WiFi: ---"

**Solutions**:

1. **Verify WiFi Credentials**
   - Open `src/config.h`
   - Check `WIFI_SSID` and `WIFI_PASSWORD` are correct
   - SSID is case-sensitive
   - Password must be exact

2. **Check WiFi Band**
   - ESP32 only supports 2.4GHz WiFi
   - 5GHz networks will NOT work
   - Set your router to broadcast 2.4GHz or use a 2.4GHz-only network

3. **Signal Strength**
   - Move ESP32 closer to router
   - Avoid metal enclosures that block WiFi
   - Check for interference from other devices

4. **Router Settings**
   - Ensure router isn't blocking new devices
   - Check MAC address filtering is disabled or add ESP32's MAC
   - Try rebooting your router

5. **Hidden Network**
   - If using a hidden SSID, ensure SSID is typed exactly as configured
   - Consider unhiding the network for testing

**Still not working?**
```cpp
// Add debug output to src/config.h
#define DEBUG_WIFI true

// Then monitor serial output:
pio device monitor
```

### WiFi Connects But Receiver Not Receiving

**Symptoms**: WiFi shows connected on controller, but PC receiver doesn't respond

**Solutions**:

1. **Verify PC IP Address**
   ```bash
   # Windows
   ipconfig

   # Look for IPv4 Address
   # Update PC_IP_ADDRESS in src/config.h to match
   ```

2. **Check Same Network**
   - Ensure ESP32 and PC are on same WiFi network
   - Some routers isolate WiFi devices (AP isolation)
   - Try connecting PC via Ethernet or same WiFi band

3. **Firewall Blocking UDP**
   - Windows Firewall may block Python
   - Add exception for Python and port 5006

4. **Port Already in Use**
   - Another application might use port 5006
   - Check with: `netstat -ano | findstr 5006`
   - Change port in both receiver and ESP32 code if needed

### Intermittent Disconnections

**Symptoms**: Controller randomly disconnects and reconnects

**Solutions**:

1. **Power Supply Issues**
   - Use quality USB cable
   - Use 5V 2A+ power supply
   - Avoid USB hubs without external power

2. **WiFi Congestion**
   - Change router WiFi channel
   - Reduce number of devices on network
   - Update router firmware

3. **Code Timeout**
   - Adjust keep-alive settings in main.cpp
   - Increase WiFi reconnection timeout

## ESP32 Upload Issues

### "Serial Port Not Found"

**Solutions**:

1. **Install USB Drivers**
   - CP210x drivers: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
   - CH340 drivers: http://www.wch-ic.com/downloads/CH341SER_ZIP.html

2. **Check USB Cable**
   - Use data cable, not charge-only cable
   - Try different USB port
   - Try different cable

3. **Device Manager (Windows)**
   - Open Device Manager
   - Look for COM ports
   - If yellow warning, update driver

### "Flash Write Error"

**Solutions**:

1. **Hold BOOT Button**
   - Press and hold BOOT button on ESP32
   - Click Upload in PlatformIO
   - Release BOOT when "Connecting..." appears

2. **Reset Before Upload**
   - Press RST button
   - Immediately start upload

3. **Check Flash Size**
   - Verify board has 4MB+ flash
   - Check `platformio.ini` matches your board

### "Out of Memory" During Compilation

**Solutions**:

1. **Reduce Build Flags**
   - Comment out unnecessary features in `platformio.ini`
   - Reduce LVGL buffer size in code

2. **Use Partition Scheme**
   - Already set to `huge_app.csv`
   - Verify partition file exists

## Python Receiver Issues

### "ModuleNotFoundError: No module named 'pynput'"

**Solution**:
```bash
pip install -r requirements.txt

# Or install individually:
pip install pynput pystray Pillow
```

### "Permission Denied" on Linux/Mac

**Solution**:
```bash
# Linux - add user to input group
sudo usermod -a -G input $USER
# Logout and login again

# Mac - grant accessibility permissions
# System Preferences > Security & Privacy > Privacy > Accessibility
# Add Python or Terminal
```

### Receiver Starts But Commands Don't Work

**Solutions**:

1. **Check Receiver Output**
   - Should show "Key press: X" or "Mouse move: X,Y"
   - If no output, check firewall and network

2. **Verify Key Mappings**
   - Check `KEY_MAP` in `gspro_receiver.py`
   - Matches ESP32 key codes

3. **Test Mouse Movement**
   - Move finger on touchpad screen
   - Should see "Mouse move: dx=X, dy=Y" in receiver
   - Cursor should move on screen

### System Tray Icon Not Appearing

**Solution**:
```bash
# Ensure dependencies installed
pip install pystray Pillow

# Check system tray is enabled in Windows
# Settings > Personalization > Taskbar > Select which icons appear on taskbar
```

## Display Issues

### Display is White/Black/Garbled

**Solutions**:

1. **Check Pin Configuration**
   - Verify pins in `platformio.ini` match your board
   - Default WT32-SC01 pins:
     ```
     TFT_MISO=12, TFT_MOSI=13, TFT_SCLK=14
     TFT_CS=15, TFT_DC=21, TFT_RST=22, TFT_BL=23
     ```

2. **Reset Display**
   - Power cycle ESP32
   - Press RST button

3. **Check Orientation**
   - May need to adjust rotation in code
   - WT32-SC01 uses landscape mode (480x320)

### Display Flickers

**Solutions**:

1. **Power Supply**
   - Use higher current supply (2A+)
   - Check for voltage drop

2. **SPI Speed**
   - Reduce `SPI_FREQUENCY` in `platformio.ini`
   - Default: 40MHz, try 27MHz or 20MHz

### Backlight Not Working

**Solutions**:

1. **Check Backlight Pin**
   - WT32-SC01 uses pin 23
   - Some boards use pin 27
   - Verify in your board's schematic

2. **PWM Conflict**
   - Check no other code uses same PWM channel
   - Backlight uses PWM channel 0 by default

## Touch Screen Issues

### Touch Not Responding

**Solutions**:

1. **I2C Configuration**
   - Default I2C speed: 400kHz
   - If unstable, reduce to 100kHz in code

2. **Touch Calibration**
   - May need to calibrate touch coordinates
   - Check FT6336U driver initialization

3. **Check I2C Pins**
   - WT32-SC01: SDA=18, SCL=19
   - Verify in your board documentation

### Touch Offset/Inaccurate

**Solutions**:

1. **Coordinate Mapping**
   - May need to swap X/Y coordinates
   - Check rotation setting matches display

2. **Add Calibration**
   - Implement touch calibration routine
   - Map touch coordinates to display coordinates

### Ghost Touches

**Solutions**:

1. **Grounding**
   - Ensure ESP32 is properly grounded
   - Use shielded USB cable

2. **Noise Filtering**
   - Add capacitor between touch IC and ground
   - Increase debounce delay in code

## Performance Issues

### UI Laggy/Slow

**Solutions**:

1. **Increase CPU Speed**
   - ESP32 should run at 240MHz
   - Check `platformio.ini` doesn't limit CPU speed

2. **Reduce LVGL Animations**
   - Disable animations in button callbacks
   - Reduce animation time

3. **Optimize Display Updates**
   - Call `lv_task_handler()` more frequently
   - Reduce buffer swapping overhead

### Memory Leaks

**Solutions**:

1. **Monitor Free Heap**
   ```cpp
   Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
   ```

2. **Check LVGL Objects**
   - Ensure deleted objects are freed
   - Use `lv_obj_del()` properly

3. **Enable PSRAM**
   - Already enabled in `platformio.ini`
   - Verify board has PSRAM: `ESP.getFreePsram()`

### High Latency

**Solutions**:

1. **WiFi Optimization**
   - Reduce WiFi power saving
   - Use QoS settings on router

2. **UDP Optimization**
   - Batch multiple commands
   - Reduce packet size

## Hardware Issues

### ESP32 Gets Hot

**Solutions**:

1. **Normal Behavior**
   - ESP32 can reach 60-70°C during WiFi operation
   - Add heatsink if concerned

2. **Check Short Circuits**
   - Inspect board for solder bridges
   - Check for damaged components

### Touch Screen Cracked

**Solution**:
- Replacement displays available online
- Search "WT32-SC01" or "3.5 inch ESP32 touch display"

### No Power LED

**Solutions**:

1. **Check Power Supply**
   - Measure voltage at 5V pin
   - Should be 4.75V-5.25V

2. **LED May Be Disabled**
   - Some boards don't have power LED
   - Check schematic

## Advanced Debugging

### Enable Serial Debug Output

Add to `src/main.cpp`:
```cpp
#define DEBUG_MODE 1

void setup() {
  Serial.begin(115200);
  Serial.println("GSPRO Controller Starting...");
  // ... rest of setup
}
```

Monitor output:
```bash
pio device monitor
```

### Check WiFi Diagnostics

```cpp
void printWiFiDiagnostics() {
  Serial.printf("WiFi Status: %d\n", WiFi.status());
  Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("Signal Strength: %d dBm\n", WiFi.RSSI());
  Serial.printf("MAC Address: %s\n", WiFi.macAddress().c_str());
}
```

### Wireshark Packet Capture

Monitor UDP traffic:

1. Install Wireshark
2. Capture on WiFi interface
3. Filter: `udp.port == 5006`
4. Verify packets from ESP32 to PC

### Test Network Connectivity

```bash
# Ping ESP32 from PC (get IP from display)
ping 192.168.1.XXX

# Check UDP port is listening
netstat -ano | findstr 5006
```

### Check ESP32 Flash Content

```bash
# Read flash
pio run --target read_flash

# Erase flash (if corrupted)
pio run --target erase

# Re-upload
pio run --target upload
```

### Factory Reset

Complete reset procedure:

1. **Erase Flash**
   ```bash
   pio run --target erase
   ```

2. **Re-upload Firmware**
   ```bash
   pio run --target upload
   ```

3. **Update WiFi Config**
   - Edit `src/config.h`
   - Rebuild and upload

## Still Having Issues?

If none of these solutions work:

1. **Open an Issue**
   - Visit: https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/issues
   - Provide:
     - Detailed description
     - Steps to reproduce
     - Serial output
     - Hardware details
     - Screenshots

2. **Join Discussion**
   - Check existing discussions
   - Ask community for help

3. **Check for Updates**
   - Pull latest code
   - Check CHANGELOG.md for fixes

## Useful Commands

```bash
# PlatformIO
pio run                    # Build
pio run --target upload    # Upload to ESP32
pio device monitor         # Serial monitor
pio run --target clean     # Clean build files

# Python
pip list                   # Show installed packages
pip install --upgrade -r requirements.txt  # Update packages

# Network
ipconfig                   # Windows - get IP
ifconfig                   # Linux/Mac - get IP
netstat -ano | findstr 5006  # Check port usage
```

## Quick Diagnostic Checklist

- [ ] WiFi credentials correct in `src/config.h`
- [ ] ESP32 on 2.4GHz WiFi network
- [ ] PC IP address correct in `src/config.h`
- [ ] Python receiver running on PC
- [ ] Firewall allows Python/UDP port 5006
- [ ] ESP32 and PC on same network
- [ ] USB cable is data cable, not charge-only
- [ ] Power supply provides 2A+
- [ ] Serial monitor shows no errors
- [ ] Latest code from repository

If all checked and still having issues, open a GitHub issue with details!
