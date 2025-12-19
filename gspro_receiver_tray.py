#!/usr/bin/env python3
"""
GSPRO Controller WiFi Receiver with System Tray
Receives keyboard and mouse commands from ESP32 over UDP and simulates input on Windows.
Runs in system tray with connection status.
"""

import socket
import struct
import threading
import time
from pynput.keyboard import Controller as KeyboardController, Key
from pynput.mouse import Controller as MouseController, Button
from pystray import Icon, Menu, MenuItem
from PIL import Image, ImageDraw

# Configuration
UDP_IP = "0.0.0.0"
UDP_PORT = 5006

# Initialize controllers
keyboard = KeyboardController()
mouse = MouseController()

# Status tracking
status = {
    'running': True,
    'connected': False,
    'last_message': 0,
    'client_ip': None,
    'local_ip': None,
    'message_count': 0
}

# Key mapping
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

def get_local_ip():
    """Get local IP address"""
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip
    except:
        return "Unknown"

def create_icon(connected=False):
    """Create system tray icon"""
    # Create a 64x64 image
    img = Image.new('RGB', (64, 64), color=(0, 0, 0))
    draw = ImageDraw.Draw(img)

    # Draw a circle
    color = (0, 230, 118) if connected else (244, 67, 54)  # Green if connected, Red if not
    draw.ellipse([8, 8, 56, 56], fill=color, outline=(255, 255, 255))

    # Draw a "G" in the center
    draw.text((20, 20), "G", fill=(255, 255, 255))

    return img

def handle_keyboard_press(key_code):
    """Handle keyboard key press"""
    try:
        if key_code in KEY_MAP:
            keyboard.press(KEY_MAP[key_code])
        else:
            keyboard.press(chr(key_code))
    except:
        pass

def handle_keyboard_release(key_code):
    """Handle keyboard key release"""
    try:
        if key_code in KEY_MAP:
            keyboard.release(KEY_MAP[key_code])
        else:
            keyboard.release(chr(key_code))
    except:
        pass

def handle_keyboard_write(key_code):
    """Handle keyboard key write"""
    try:
        if key_code in KEY_MAP:
            keyboard.press(KEY_MAP[key_code])
            keyboard.release(KEY_MAP[key_code])
        else:
            keyboard.press(chr(key_code))
            keyboard.release(chr(key_code))
    except:
        pass

def handle_mouse_move(dx, dy):
    """Handle mouse movement"""
    try:
        current_x, current_y = mouse.position
        mouse.position = (current_x + dx, current_y + dy)
    except:
        pass

def handle_mouse_click(button_code):
    """Handle mouse click"""
    try:
        button = Button.left if button_code == 1 else Button.right if button_code == 2 else Button.middle
        mouse.click(button)
    except:
        pass

def handle_mouse_press(button_code):
    """Handle mouse button press"""
    try:
        button = Button.left if button_code == 1 else Button.right if button_code == 2 else Button.middle
        mouse.press(button)
    except:
        pass

def handle_mouse_release(button_code):
    """Handle mouse button release"""
    try:
        button = Button.left if button_code == 1 else Button.right if button_code == 2 else Button.middle
        mouse.release(button)
    except:
        pass

def process_command(data, addr):
    """Process incoming UDP command"""
    if len(data) < 2:
        return

    # Update connection status
    status['last_message'] = time.time()
    status['connected'] = True
    status['client_ip'] = addr[0]
    status['message_count'] += 1

    cmd_type = data[0]

    if cmd_type == 1:  # Keyboard Press
        handle_keyboard_press(data[1])
    elif cmd_type == 2:  # Keyboard Release
        handle_keyboard_release(data[1])
    elif cmd_type == 3:  # Keyboard Write
        handle_keyboard_write(data[1])
    elif cmd_type == 4:  # Mouse Move
        if len(data) >= 3:
            dx = struct.unpack('b', bytes([data[1]]))[0]
            dy = struct.unpack('b', bytes([data[2]]))[0]
            handle_mouse_move(dx, dy)
    elif cmd_type == 5:  # Mouse Click
        handle_mouse_click(data[1])
    elif cmd_type == 6:  # Mouse Press
        handle_mouse_press(data[1])
    elif cmd_type == 7:  # Mouse Release
        handle_mouse_release(data[1])

def udp_server():
    """UDP server thread"""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(1.0)  # 1 second timeout for checking status['running']
    sock.bind((UDP_IP, UDP_PORT))

    print(f"GSPRO Controller WiFi Receiver")
    print(f"Listening on {UDP_IP}:{UDP_PORT}")
    print(f"Local IP: {status['local_ip']}")
    print(f"Running in system tray...")
    print("-" * 50)

    while status['running']:
        try:
            data, addr = sock.recvfrom(1024)
            process_command(data, addr)
        except socket.timeout:
            # Check if we should mark as disconnected (no message in 5 seconds)
            if status['connected'] and (time.time() - status['last_message']) > 5:
                status['connected'] = False
                status['client_ip'] = None
        except Exception as e:
            if status['running']:
                print(f"Error: {e}")

    sock.close()

def get_status_text(item=None):
    """Get status text for menu"""
    if status['connected'] and status['client_ip']:
        return f"Connected: {status['client_ip']}"
    else:
        return "Waiting for controller..."

def on_quit(icon, item):
    """Handle quit menu item"""
    status['running'] = False
    icon.stop()

def on_show_status(icon, item):
    """Show status window (placeholder)"""
    import tkinter as tk
    from tkinter import messagebox

    root = tk.Tk()
    root.withdraw()

    msg = f"""GSPRO Controller WiFi Receiver

Status: {'Connected' if status['connected'] else 'Waiting...'}
Local IP: {status['local_ip']}
Port: {UDP_PORT}
Client IP: {status['client_ip'] if status['client_ip'] else 'None'}
Messages: {status['message_count']}
"""

    messagebox.showinfo("GSPRO Controller Status", msg)
    root.destroy()

def update_icon(icon):
    """Update icon based on connection status"""
    while status['running']:
        # Update icon color based on connection
        icon.icon = create_icon(status['connected'])

        # Update menu title
        icon.title = f"GSPRO Controller\n{get_status_text()}"

        time.sleep(1)

def main():
    """Main function"""
    # Get local IP
    status['local_ip'] = get_local_ip()

    # Start UDP server thread
    server_thread = threading.Thread(target=udp_server, daemon=True)
    server_thread.start()

    # Create system tray icon
    icon_image = create_icon(False)

    menu = Menu(
        MenuItem(get_status_text, lambda: None, enabled=False),
        MenuItem(f"Local IP: {status['local_ip']}", lambda: None, enabled=False),
        Menu.SEPARATOR,
        MenuItem("Show Status", on_show_status),
        MenuItem("Exit", on_quit)
    )

    icon = Icon("GSPRO Controller", icon_image, "GSPRO Controller", menu)

    # Start icon updater thread
    updater_thread = threading.Thread(target=lambda: update_icon(icon), daemon=True)
    updater_thread.start()

    # Run the icon (blocks until quit)
    icon.run()

    # Wait for server thread to finish
    server_thread.join(timeout=2)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nShutting down...")
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()
