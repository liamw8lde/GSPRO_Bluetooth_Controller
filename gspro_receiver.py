#!/usr/bin/env python3
"""
GSPRO Controller WiFi Receiver
Receives keyboard and mouse commands from ESP32 over UDP and simulates input on Windows.
"""

import socket
import struct
from pynput.keyboard import Controller as KeyboardController, Key
from pynput.mouse import Controller as MouseController, Button

# Configuration
UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 5006  # Changed from 5005 due to Windows Media Player conflict

# Initialize controllers
keyboard = KeyboardController()
mouse = MouseController()

# Key mapping (matches ESP32 side)
KEY_MAP = {
    0x80: Key.ctrl_l,      # KEY_LEFT_CTRL
    0x81: Key.shift_l,     # KEY_LEFT_SHIFT
    0x82: Key.alt_l,       # KEY_LEFT_ALT
    0xDA: Key.up,          # KEY_UP_ARROW
    0xD9: Key.down,        # KEY_DOWN_ARROW
    0xD8: Key.left,        # KEY_LEFT_ARROW
    0xD7: Key.right,       # KEY_RIGHT_ARROW
    0xC6: Key.f5,          # KEY_F5
}

def handle_keyboard_press(key_code):
    """Handle keyboard key press"""
    if key_code in KEY_MAP:
        keyboard.press(KEY_MAP[key_code])
    else:
        # Regular ASCII character
        keyboard.press(chr(key_code))
    print(f"Key press: {key_code}")

def handle_keyboard_release(key_code):
    """Handle keyboard key release"""
    if key_code in KEY_MAP:
        keyboard.release(KEY_MAP[key_code])
    else:
        keyboard.release(chr(key_code))
    print(f"Key release: {key_code}")

def handle_keyboard_write(key_code):
    """Handle keyboard key write (press and release)"""
    if key_code in KEY_MAP:
        keyboard.press(KEY_MAP[key_code])
        keyboard.release(KEY_MAP[key_code])
    else:
        keyboard.press(chr(key_code))
        keyboard.release(chr(key_code))
    print(f"Key write: {key_code}")

def handle_mouse_move(dx, dy):
    """Handle mouse movement (relative)"""
    current_x, current_y = mouse.position
    mouse.position = (current_x + dx, current_y + dy)
    print(f"Mouse move: dx={dx}, dy={dy}")

def handle_mouse_click(button_code):
    """Handle mouse click"""
    button = Button.left if button_code == 1 else Button.right if button_code == 2 else Button.middle
    mouse.click(button)
    print(f"Mouse click: {button_code}")

def handle_mouse_press(button_code):
    """Handle mouse button press"""
    button = Button.left if button_code == 1 else Button.right if button_code == 2 else Button.middle
    mouse.press(button)
    print(f"Mouse press: {button_code}")

def handle_mouse_release(button_code):
    """Handle mouse button release"""
    button = Button.left if button_code == 1 else Button.right if button_code == 2 else Button.middle
    mouse.release(button)
    print(f"Mouse release: {button_code}")

def process_command(data):
    """Process incoming UDP command"""
    if len(data) < 2:
        return

    cmd_type = data[0]

    # Command types:
    # 1 = Keyboard Press
    # 2 = Keyboard Release
    # 3 = Keyboard Write
    # 4 = Mouse Move
    # 5 = Mouse Click
    # 6 = Mouse Press
    # 7 = Mouse Release

    if cmd_type == 1:  # Keyboard Press
        key_code = data[1]
        handle_keyboard_press(key_code)

    elif cmd_type == 2:  # Keyboard Release
        key_code = data[1]
        handle_keyboard_release(key_code)

    elif cmd_type == 3:  # Keyboard Write
        key_code = data[1]
        handle_keyboard_write(key_code)

    elif cmd_type == 4:  # Mouse Move
        if len(data) >= 3:
            dx = struct.unpack('b', bytes([data[1]]))[0]  # signed byte
            dy = struct.unpack('b', bytes([data[2]]))[0]  # signed byte
            handle_mouse_move(dx, dy)

    elif cmd_type == 5:  # Mouse Click
        button_code = data[1]
        handle_mouse_click(button_code)

    elif cmd_type == 6:  # Mouse Press
        button_code = data[1]
        handle_mouse_press(button_code)

    elif cmd_type == 7:  # Mouse Release
        button_code = data[1]
        handle_mouse_release(button_code)

def main():
    """Main server loop"""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print(f"GSPRO Controller WiFi Receiver")
    print(f"Listening on {UDP_IP}:{UDP_PORT}")
    print(f"Waiting for ESP32 controller...")
    print("-" * 50)

    while True:
        data, addr = sock.recvfrom(1024)
        process_command(data)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nShutting down...")
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()
