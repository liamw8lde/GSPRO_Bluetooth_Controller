# API Reference - UDP Protocol Specification

This document describes the UDP communication protocol used between the ESP32 controller and the PC receiver.

## Table of Contents

- [Overview](#overview)
- [Network Configuration](#network-configuration)
- [Packet Structure](#packet-structure)
- [Command Types](#command-types)
- [Keyboard Commands](#keyboard-commands)
- [Mouse Commands](#mouse-commands)
- [Implementation Examples](#implementation-examples)
- [Error Handling](#error-handling)

## Overview

The GSPRO Controller uses UDP (User Datagram Protocol) for wireless communication between the ESP32 device and PC receiver. This provides:

- Low latency for real-time input
- Simple implementation
- No connection overhead
- Reliable local network transmission

### Protocol Characteristics

- **Transport**: UDP (connectionless)
- **Port**: 5006
- **Packet Size**: 2-3 bytes per command
- **Byte Order**: Little-endian
- **Direction**: Unidirectional (ESP32 → PC)

## Network Configuration

### Default Settings

```cpp
// ESP32 Configuration (src/config.h)
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define PC_IP_ADDRESS "192.168.1.100"
#define UDP_PORT 5006
```

```python
# Python Receiver Configuration (gspro_receiver.py)
UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 5006
```

### Network Requirements

- Both devices must be on the same local network
- 2.4GHz WiFi (ESP32 limitation)
- UDP port 5006 must not be blocked by firewall
- Static or DHCP IP addressing supported

## Packet Structure

All packets follow this basic structure:

```
+--------+--------+--------+
| Byte 0 | Byte 1 | Byte 2 |
+--------+--------+--------+
|  CMD   | PARAM1 | PARAM2 |
+--------+--------+--------+
```

### Field Descriptions

| Field | Size | Description |
|-------|------|-------------|
| CMD | 1 byte | Command type (1-7) |
| PARAM1 | 1 byte | First parameter (key code, button, or dx) |
| PARAM2 | 1 byte | Second parameter (optional, used for mouse dy) |

## Command Types

| Command ID | Name | Parameters | Packet Size | Description |
|-----------|------|------------|-------------|-------------|
| 1 | Keyboard Press | key_code | 2 bytes | Press key without releasing |
| 2 | Keyboard Release | key_code | 2 bytes | Release previously pressed key |
| 3 | Keyboard Write | key_code | 2 bytes | Press and immediately release key |
| 4 | Mouse Move | dx, dy | 3 bytes | Move mouse by relative coordinates |
| 5 | Mouse Click | button_code | 2 bytes | Click and release mouse button |
| 6 | Mouse Press | button_code | 2 bytes | Press mouse button without releasing |
| 7 | Mouse Release | button_code | 2 bytes | Release previously pressed mouse button |

## Keyboard Commands

### Command 1: Keyboard Press

Press a key and hold it down.

**Format**:
```
[0x01] [key_code]
```

**Example** (Press Ctrl):
```
0x01 0x80
```

**Use Cases**:
- Modifier keys (Ctrl, Shift, Alt)
- Key combinations
- Hold-to-repeat actions

### Command 2: Keyboard Release

Release a previously pressed key.

**Format**:
```
[0x02] [key_code]
```

**Example** (Release Ctrl):
```
0x02 0x80
```

### Command 3: Keyboard Write

Press and immediately release a key (most common).

**Format**:
```
[0x03] [key_code]
```

**Example** (Type 'P' for Pin):
```
0x03 0x50
```

### Key Codes

#### Standard ASCII Characters
Use ASCII values for regular characters:
- 'A'-'Z': 0x41-0x5A (uppercase)
- 'a'-'z': 0x61-0x7A (lowercase)
- '0'-'9': 0x30-0x39

#### Special Key Codes

| Key | Code | Hex | Description |
|-----|------|-----|-------------|
| KEY_LEFT_CTRL | 128 | 0x80 | Left Control |
| KEY_LEFT_SHIFT | 129 | 0x81 | Left Shift |
| KEY_LEFT_ALT | 130 | 0x82 | Left Alt |
| KEY_UP_ARROW | 218 | 0xDA | Up Arrow |
| KEY_DOWN_ARROW | 217 | 0xD9 | Down Arrow |
| KEY_LEFT_ARROW | 216 | 0xD8 | Left Arrow |
| KEY_RIGHT_ARROW | 215 | 0xD7 | Right Arrow |
| KEY_F5 | 198 | 0xC6 | F5 Function Key |

#### GSPRO-Specific Mappings

| Action | Keys | Codes | Packet Sequence |
|--------|------|-------|-----------------|
| Mulligan | Ctrl+M | 0x80, 0x4D | Press Ctrl, Press M, Release M, Release Ctrl |
| Pin | P | 0x50 | Write P |
| Scout | J | 0x4A | Write J |
| Heat Map | Z | 0x5A | Write Z |
| Flyover | O | 0x4F | Write O |
| Free Camera | F5 | 0xC6 | Write F5 |
| Up Arrow | ↑ | 0xDA | Write Up |
| Down Arrow | ↓ | 0xD9 | Write Down |
| Left Arrow | ← | 0xD8 | Write Left |
| Right Arrow | → | 0xD7 | Write Right |
| Tee Left | C | 0x43 | Write C |
| Tee Right | V | 0x56 | Write V |

## Mouse Commands

### Command 4: Mouse Move

Move mouse cursor by relative coordinates.

**Format**:
```
[0x04] [dx] [dy]
```

**Parameters**:
- `dx`: Horizontal movement (-128 to +127 pixels)
- `dy`: Vertical movement (-128 to +127 pixels)

**Example** (Move right 10, down 5):
```
0x04 0x0A 0x05
```

**Example** (Move left 10, up 5):
```
0x04 0xF6 0xFB  // -10 = 0xF6, -5 = 0xFB (two's complement)
```

**Notes**:
- Coordinates are signed bytes (two's complement)
- Positive dx = right, negative dx = left
- Positive dy = down, negative dy = up
- Movement is relative to current cursor position

### Command 5: Mouse Click

Click (press and release) a mouse button.

**Format**:
```
[0x05] [button_code]
```

**Button Codes**:
- `1`: Left button
- `2`: Right button
- `3`: Middle button

**Example** (Left click):
```
0x05 0x01
```

### Command 6: Mouse Press

Press a mouse button without releasing.

**Format**:
```
[0x06] [button_code]
```

**Example** (Press left button):
```
0x06 0x01
```

### Command 7: Mouse Release

Release a previously pressed mouse button.

**Format**:
```
[0x07] [button_code]
```

**Example** (Release left button):
```
0x07 0x01
```

## Implementation Examples

### ESP32 (C++) - Sending Commands

```cpp
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;
const char* pc_ip = "192.168.1.100";
const int udp_port = 5006;

// Send keyboard write command
void sendKey(uint8_t key_code) {
  uint8_t packet[2] = {0x03, key_code};  // CMD=3 (Write), PARAM=key_code
  udp.beginPacket(pc_ip, udp_port);
  udp.write(packet, 2);
  udp.endPacket();
}

// Send key combination (Ctrl+M for Mulligan)
void sendMulligan() {
  uint8_t packet[2];

  // Press Ctrl
  packet[0] = 0x01;  // CMD=Press
  packet[1] = 0x80;  // KEY_LEFT_CTRL
  udp.beginPacket(pc_ip, udp_port);
  udp.write(packet, 2);
  udp.endPacket();

  delay(10);

  // Press M
  packet[0] = 0x01;  // CMD=Press
  packet[1] = 0x4D;  // 'M'
  udp.beginPacket(pc_ip, udp_port);
  udp.write(packet, 2);
  udp.endPacket();

  delay(10);

  // Release M
  packet[0] = 0x02;  // CMD=Release
  packet[1] = 0x4D;
  udp.beginPacket(pc_ip, udp_port);
  udp.write(packet, 2);
  udp.endPacket();

  delay(10);

  // Release Ctrl
  packet[0] = 0x02;  // CMD=Release
  packet[1] = 0x80;
  udp.beginPacket(pc_ip, udp_port);
  udp.write(packet, 2);
  udp.endPacket();
}

// Send mouse movement
void sendMouseMove(int8_t dx, int8_t dy) {
  uint8_t packet[3] = {0x04, (uint8_t)dx, (uint8_t)dy};
  udp.beginPacket(pc_ip, udp_port);
  udp.write(packet, 3);
  udp.endPacket();
}

// Send mouse click
void sendMouseClick(uint8_t button) {
  uint8_t packet[2] = {0x05, button};
  udp.beginPacket(pc_ip, udp_port);
  udp.write(packet, 2);
  udp.endPacket();
}
```

### Python - Receiving Commands

```python
import socket
import struct
from pynput.keyboard import Controller as KeyboardController, Key
from pynput.mouse import Controller as MouseController, Button

UDP_PORT = 5006
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", UDP_PORT))

keyboard = KeyboardController()
mouse = MouseController()

KEY_MAP = {
    0x80: Key.ctrl_l,
    0x81: Key.shift_l,
    0x82: Key.alt_l,
    0xDA: Key.up,
    0xD9: Key.down,
    0xD8: Key.left,
    0xD7: Key.right,
    0xC6: Key.f5,
}

while True:
    data, addr = sock.recvfrom(1024)

    if len(data) < 2:
        continue

    cmd = data[0]

    if cmd == 1:  # Keyboard Press
        key = data[1]
        if key in KEY_MAP:
            keyboard.press(KEY_MAP[key])
        else:
            keyboard.press(chr(key))

    elif cmd == 2:  # Keyboard Release
        key = data[1]
        if key in KEY_MAP:
            keyboard.release(KEY_MAP[key])
        else:
            keyboard.release(chr(key))

    elif cmd == 3:  # Keyboard Write
        key = data[1]
        if key in KEY_MAP:
            keyboard.press(KEY_MAP[key])
            keyboard.release(KEY_MAP[key])
        else:
            keyboard.press(chr(key))
            keyboard.release(chr(key))

    elif cmd == 4:  # Mouse Move
        if len(data) >= 3:
            dx = struct.unpack('b', bytes([data[1]]))[0]
            dy = struct.unpack('b', bytes([data[2]]))[0]
            current_x, current_y = mouse.position
            mouse.position = (current_x + dx, current_y + dy)

    elif cmd == 5:  # Mouse Click
        button = Button.left if data[1] == 1 else Button.right if data[1] == 2 else Button.middle
        mouse.click(button)

    elif cmd == 6:  # Mouse Press
        button = Button.left if data[1] == 1 else Button.right if data[1] == 2 else Button.middle
        mouse.press(button)

    elif cmd == 7:  # Mouse Release
        button = Button.left if data[1] == 1 else Button.right if data[1] == 2 else Button.middle
        mouse.release(button)
```

## Error Handling

### Packet Loss

UDP does not guarantee delivery. For critical commands:

1. **User Feedback**: Show visual feedback on controller
2. **Retry Logic**: Retry important commands
3. **Acknowledgment**: Implement ACK packets if needed

### Network Errors

Handle common network errors:

```cpp
// ESP32: Check WiFi connection before sending
if (WiFi.status() != WL_CONNECTED) {
  reconnectWiFi();
  return;
}
```

```python
# Python: Handle socket errors
try:
    data, addr = sock.recvfrom(1024)
except socket.timeout:
    print("No data received")
except Exception as e:
    print(f"Socket error: {e}")
```

### Invalid Commands

Validate command types:

```python
# Python: Validate command range
if cmd < 1 or cmd > 7:
    print(f"Invalid command: {cmd}")
    continue
```

## Performance Considerations

### Packet Rate

- **Maximum**: ~100 packets/second
- **Typical**: 10-30 packets/second
- **Mouse movement**: Can batch movements for smoothness

### Latency

Expected latency components:
- WiFi transmission: 1-5ms
- Processing: <1ms
- Total: 2-10ms typical

### Bandwidth

Each command uses minimal bandwidth:
- Keyboard: 2 bytes
- Mouse: 3 bytes
- At 30 commands/sec: ~60-90 bytes/sec

## Security Considerations

### Current Implementation

- No encryption (local network only)
- No authentication (trust local network)
- No message integrity checks

### Recommendations for Production

If deploying beyond local network:

1. **Encryption**: Use DTLS over UDP
2. **Authentication**: Validate sender MAC/IP
3. **Firewall**: Block external access to UDP port
4. **Rate Limiting**: Prevent command flooding

## Future Extensions

Potential protocol enhancements:

1. **Bidirectional Communication**: PC → ESP32 status updates
2. **Acknowledgments**: Confirm command receipt
3. **Batch Commands**: Multiple commands in single packet
4. **Configuration**: Send config changes over UDP
5. **Heartbeat**: Keep-alive packets for connection monitoring

## Testing

### Manual Testing

```bash
# Send test packet using netcat (Linux/Mac)
echo -ne '\x03\x50' | nc -u 192.168.1.100 5006

# Windows PowerShell
$udp = New-Object System.Net.Sockets.UdpClient
$udp.Connect("192.168.1.100", 5006)
$bytes = [byte[]]@(0x03, 0x50)
$udp.Send($bytes, $bytes.Length)
```

### Wireshark Capture

Filter for UDP port 5006:
```
udp.port == 5006
```

Inspect packet hex dump to verify structure.

## Reference Implementation

See source files:
- ESP32 sender: `src/main.cpp`
- Python receiver: `gspro_receiver.py`
- System tray version: `gspro_receiver_tray.py`

## Questions?

For protocol questions or suggestions, open an issue:
https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/issues
