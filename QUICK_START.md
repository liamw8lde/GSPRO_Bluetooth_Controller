# GSPRO Controller WiFi - Quick Start Guide

## ✅ What We Did

Switched from **Bluetooth HID** (which has Windows compatibility issues) to **WiFi UDP** communication for reliable keyboard and mouse control!

## 📋 Setup Steps

### Step 1: Configure WiFi Credentials

Edit: `GSPRO_Bluetooth_Controller/src/config.h`

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"          // Your WiFi network name
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"  // Your WiFi password
#define PC_IP_ADDRESS "192.168.1.100"       // Your PC's IP address (see below)
```

### Step 2: Find Your PC's IP Address

**Windows:**
```bash
ipconfig
```
Look for "IPv4 Address" under your WiFi or Ethernet adapter.
Example: `192.168.1.100`

**Write this down and update `config.h`!**

### Step 3: Install Python on Your PC

1. Download Python 3.8+ from https://python.org/downloads/
2. Run installer, **check "Add Python to PATH"**
3. Open Command Prompt:
```bash
cd "C:\Projects\GSpro controller"
pip install pynput
```

### Step 4: Upload to ESP32

```bash
cd "C:\Projects\GSpro controller\GSPRO_Bluetooth_Controller"
python -m platformio run --target upload
```

### Step 5: Run the PC Receiver

Open Command Prompt:
```bash
cd "C:\Projects\GSpro controller"
python gspro_receiver.py
```

You should see:
```
GSPRO Controller WiFi Receiver
Listening on 0.0.0.0:5005
Waiting for ESP32 controller...
```

### Step 6: Power On Controller

1. Power on your WT32-SC01 controller
2. Wait for WiFi connection (status shows on screen)
3. Header will show: `WiFi: 192.168.1.XXX` in green when connected
4. Use the controller - keyboard and **MOUSE NOW WORK!** 🎉

## 🎮 Using the Controller

- **All buttons work** as before for golf simulator controls
- **MOUSE button** (top right) - Opens touchpad mode
- **Touchpad mode** - Finger movements move the mouse cursor
- **Left/Right click buttons** work in touchpad mode

## 🔧 Troubleshooting

**WiFi won't connect:**
- Double-check SSID and password in `config.h`
- Make sure ESP32 is close to WiFi router
- Check WiFi is 2.4GHz (ESP32 doesn't support 5GHz)

**Python receiver not receiving:**
- Verify PC IP address is correct in `config.h`
- Check Windows Firewall isn't blocking Python
- Make sure both devices are on same WiFi network

**Allow Python through Windows Firewall:**
1. Windows Security → Firewall & network protection
2. Allow an app through firewall
3. Find Python, check both Private and Public

## 📝 Files Created

- `gspro_receiver.py` - Python receiver app for PC
- `requirements.txt` - Python dependencies
- `src/config.h` - WiFi configuration
- `WIFI_SETUP.md` - Detailed setup guide
- `QUICK_START.md` - This file

## 🎯 Benefits of WiFi Solution

✅ **Mouse works perfectly** on Windows (BLE didn't work)
✅ **More reliable** than Bluetooth
✅ **Better range** - Works through walls
✅ **No pairing issues**
✅ **Lower latency**
✅ **No Windows driver problems**

## 🚀 Ready to Use!

Your GSPRO Controller now has full keyboard AND mouse functionality over WiFi!

Questions? Check `WIFI_SETUP.md` for detailed troubleshooting.
