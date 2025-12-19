@echo off
echo Building GSPRO Controller Receiver Executable...
echo.

REM Install dependencies
echo Installing dependencies...
pip install -r requirements.txt

REM Build executable
echo.
echo Building executable with PyInstaller...
pyinstaller --onefile --windowed --name "GSPRO_Controller_Receiver" --icon=NONE gspro_receiver_tray.py

echo.
echo Build complete! Executable is in the 'dist' folder.
echo.
pause
