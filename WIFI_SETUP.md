# GSPRO Controller WiFi Setup Guide

## PC Setup (Windows)

### 1. Install Python
Download and install Python 3.8 or later from https://www.python.org/downloads/

### 2. Install Dependencies
Open Command Prompt and run:
```bash
cd "C:\Projects\GSpro controller"
pip install -r requirements.txt
```

### 3. Find Your PC's IP Address
Open Command Prompt and run:
```bash
ipconfig
```
Look for "IPv4 Address" under your active network adapter (WiFi or Ethernet).
Example: `192.168.1.100`

**IMPORTANT: Write down this IP address - you'll need it for ESP32 configuration!**

### 4. Run the Receiver
```bash
python gspro_receiver.py
```

You should see:
```
GSPRO Controller WiFi Receiver
Listening on 0.0.0.0:5005
Waiting for ESP32 controller...
```

## ESP32 Setup

### 1. Configure WiFi Credentials
Edit the file: `GSPRO_Bluetooth_Controller/src/config.h` and update:
- `WIFI_SSID` - Your WiFi network name
- `WIFI_PASSWORD` - Your WiFi password
- `PC_IP_ADDRESS` - Your PC's IP address from step 3 above

### 2. Upload to ESP32
The code will automatically connect to WiFi and send commands to your PC.

## Usage

1. Start the Python receiver on your PC first
2. Power on the ESP32 controller
3. Wait for WiFi connection (status shown on screen)
4. Use the controller - all keyboard and mouse commands will work!

## Troubleshooting

**Controller won't connect:**
- Check SSID and password are correct
- Make sure PC and ESP32 are on the same WiFi network
- Check Windows Firewall isn't blocking UDP port 5005

**Commands not working:**
- Verify Python receiver is running
- Check PC IP address is correct in config.h
- Make sure both devices are on the same network

**To allow through Windows Firewall:**
1. Windows Security → Firewall & network protection
2. Allow an app through firewall
3. Add Python to allowed apps
