@echo off
:: Windows Process Hiding Tool - File Opener
:: This script will show all available files in the current directory

echo ===============================================================
echo Windows Process Hiding Tool - File Explorer
echo ===============================================================
echo.
echo Available files in this directory:
echo.

dir /B

echo.
echo To view a file, type: notepad filename.extension
echo Example: notepad SETUP_GUIDE.md
echo.
echo To build the tool, run: build.bat
echo To use the tool, run: hide_process.bat process_name.exe
echo.
echo Press any key to open the setup guide...
pause >nul

:: Open setup guide
start notepad SETUP_GUIDE.md
