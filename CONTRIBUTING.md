# Contributing to GSPRO Controller

Thank you for your interest in contributing to the GSPRO Controller project! This document provides guidelines and instructions for contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Development Setup](#development-setup)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Documentation](#documentation)

## Code of Conduct

This project follows a simple code of conduct:

- Be respectful and inclusive
- Provide constructive feedback
- Focus on what is best for the community
- Show empathy towards other community members

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When creating a bug report, include:

- **Clear title and description**
- **Steps to reproduce** the behavior
- **Expected behavior** vs **actual behavior**
- **Screenshots** if applicable
- **Environment details**:
  - ESP32 board model
  - Display model
  - PlatformIO version
  - Operating system
  - WiFi router model (if relevant)

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, include:

- **Clear title and description**
- **Use case** - why is this enhancement needed?
- **Proposed solution**
- **Alternative solutions** you've considered
- **Additional context** like mockups or examples

### Your First Code Contribution

Unsure where to begin? Look for issues labeled:

- `good first issue` - Small, self-contained changes
- `help wanted` - Issues where we need community help
- `documentation` - Documentation improvements

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Test thoroughly
5. Commit with clear messages
6. Push to your fork
7. Open a Pull Request

## Development Setup

### Prerequisites

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE Extension](https://platformio.org/install/ide?install=vscode)
- Python 3.8+ (for PC receiver development)
- Git

### ESP32 Development Setup

1. Clone the repository:
```bash
git clone https://github.com/liamw8lde/GSPRO_Bluetooth_Controller.git
cd GSPRO_Bluetooth_Controller
```

2. Open in VS Code:
```bash
code .
```

3. Install PlatformIO extension if not already installed

4. Configure WiFi credentials in `src/config.h`:
```cpp
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define PC_IP_ADDRESS "192.168.1.100"
```

5. Build the project:
```bash
pio run
```

6. Upload to ESP32:
```bash
pio run --target upload
```

### Python Receiver Development

1. Create a virtual environment:
```bash
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

2. Install dependencies:
```bash
pip install -r requirements.txt
```

3. Run the receiver:
```bash
python gspro_receiver.py
```

## Pull Request Process

1. **Update Documentation**: If you change functionality, update relevant docs
2. **Follow Coding Standards**: See below
3. **Test Your Changes**: Verify on actual hardware if possible
4. **Update CHANGELOG.md**: Add your changes under "Unreleased"
5. **Clear PR Description**: Explain what, why, and how
6. **Link Related Issues**: Use "Fixes #123" or "Relates to #456"
7. **Request Review**: Tag relevant maintainers

### PR Title Format

Use conventional commits format:

- `feat: Add custom button mapping UI`
- `fix: Resolve WiFi reconnection issue`
- `docs: Update installation instructions`
- `refactor: Simplify touch handling code`
- `test: Add unit tests for UDP protocol`
- `chore: Update dependencies`

## Coding Standards

### C++ (ESP32)

- **Formatting**:
  - 2-space indentation
  - K&R brace style
  - Max line length: 100 characters

- **Naming**:
  - Classes: `PascalCase`
  - Functions: `camelCase`
  - Variables: `snake_case`
  - Constants: `UPPER_SNAKE_CASE`
  - Private members: `m_variableName`

- **Comments**:
  - Use `//` for single-line comments
  - Use `/* */` for multi-line comments
  - Document public functions with brief descriptions
  - Explain "why" not "what" in complex code

Example:
```cpp
// Good
void updateWiFiStatus() {
  // Check connection every 5 seconds to avoid overhead
  if (millis() - last_check > 5000) {
    wifi_connected = WiFi.status() == WL_CONNECTED;
  }
}

// Bad
void updateWiFiStatus() {
  // This function updates the WiFi status
  if (millis() - last_check > 5000) { // If 5 seconds passed
    wifi_connected = WiFi.status() == WL_CONNECTED; // Update status
  }
}
```

### Python (Receiver)

- **Formatting**:
  - Follow PEP 8
  - 4-space indentation
  - Max line length: 88 characters (Black formatter)

- **Type Hints**:
  - Use type hints for function parameters and returns
  - Use `typing` module for complex types

- **Docstrings**:
  - Use Google-style docstrings
  - Document all public functions

Example:
```python
def handle_mouse_move(dx: int, dy: int) -> None:
    """Handle mouse movement with relative coordinates.

    Args:
        dx: Horizontal movement delta in pixels
        dy: Vertical movement delta in pixels
    """
    current_x, current_y = mouse.position
    mouse.position = (current_x + dx, current_y + dy)
```

## Testing Guidelines

### ESP32 Testing

Test your changes on actual hardware:

1. **Basic Functionality**:
   - All buttons respond correctly
   - Touch is accurate
   - Display renders properly
   - WiFi connects reliably

2. **Edge Cases**:
   - WiFi disconnection/reconnection
   - Rapid button presses
   - Long-press actions
   - Display sleep/wake

3. **Performance**:
   - UI remains responsive
   - No memory leaks (monitor free heap)
   - Smooth touch tracking

### Python Testing

1. **Manual Testing**:
   - Test all keyboard commands
   - Test mouse movement
   - Test click actions
   - Test error handling

2. **Network Testing**:
   - Test with poor WiFi signal
   - Test with multiple clients
   - Test UDP packet loss handling

## Documentation

### When to Update Documentation

Update documentation when you:

- Add new features
- Change existing functionality
- Fix bugs that affect user experience
- Modify installation/setup process
- Change configuration options

### Documentation Files

- `README.md` - Project overview, quick start
- `QUICK_START.md` - Minimal steps to get running
- `WIFI_SETUP.md` - Detailed WiFi configuration
- `TROUBLESHOOTING.md` - Common issues and solutions
- `API_REFERENCE.md` - UDP protocol specification
- `HARDWARE.md` - Hardware details and schematics
- `CHANGELOG.md` - Version history
- `CONTRIBUTING.md` - This file

### Documentation Style

- Use clear, concise language
- Include code examples
- Add screenshots for UI changes
- Provide step-by-step instructions
- Test all instructions yourself

## Project Structure

```
GSPRO_Bluetooth_Controller/
├── src/                          # ESP32 source code
│   ├── main.cpp                  # Main application
│   ├── config.h                  # WiFi configuration
│   ├── BleCombo.h/cpp           # Legacy BLE code
│   ├── Touch.h/cpp              # Touch handling
│   └── logo_image.h             # Display assets
├── GSPRO_Bluetooth_Controller/   # PlatformIO project files
├── gspro_receiver.py            # PC receiver (console)
├── gspro_receiver_tray.py       # PC receiver (system tray)
├── platformio.ini               # PlatformIO configuration
├── requirements.txt             # Python dependencies
├── build_exe.bat               # Windows executable builder
├── README.md                    # Main documentation
├── QUICK_START.md              # Quick start guide
├── WIFI_SETUP.md               # WiFi setup guide
├── CHANGELOG.md                # Version history
├── CONTRIBUTING.md             # This file
├── TROUBLESHOOTING.md          # Troubleshooting guide
├── API_REFERENCE.md            # Protocol documentation
├── HARDWARE.md                 # Hardware specifications
└── LICENSE                     # License file
```

## Communication

### Getting Help

- **Questions**: Open a [GitHub Discussion](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/discussions)
- **Bugs**: Open an [Issue](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/issues)
- **Feature Requests**: Open an [Issue](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/issues)

### Review Process

Pull requests require:

1. At least one maintainer approval
2. Passing CI checks (if configured)
3. Up-to-date with main branch
4. Resolved conversations

Maintainers will review PRs within:

- Critical bugs: 1-2 days
- Features: 3-7 days
- Documentation: 3-7 days

## Recognition

Contributors will be:

- Listed in release notes
- Mentioned in CHANGELOG.md
- Credited in GitHub contributors

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (see LICENSE file).

## Questions?

If you have questions about contributing, feel free to:

- Open a [GitHub Discussion](https://github.com/liamw8lde/GSPRO_Bluetooth_Controller/discussions)
- Create an issue labeled `question`

Thank you for contributing to GSPRO Controller!
