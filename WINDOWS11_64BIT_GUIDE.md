# Windows 11 64-bit Process Hiding Tool - Setup Guide

This guide is specifically for Windows 11 64-bit systems to address the "Unsupported 16-Bit Application" error.

## System Requirements

- Windows 11 64-bit 
- Visual Studio with C/C++ development tools OR Visual C++ Build Tools
- Administrator privileges

## Step 1: Install Visual Studio Build Tools

If you don't already have Visual Studio or Build Tools installed:

1. Download Visual Studio Build Tools from:
   https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022

2. Run the installer and select "Desktop development with C++"

3. Complete the installation

## Step 2: Build the 64-bit Tool

1. Open the **x64 Native Tools Command Prompt for VS 2022** as Administrator:
   - Search for "x64 Native" in the Start menu
   - Right-click on "x64 Native Tools Command Prompt for VS 2022"
   - Select "Run as administrator"

2. Navigate to the directory with your files:
   ```cmd
   cd path\to\your\files
   ```

3. Build the 64-bit versions:
   ```cmd
   build_x64.bat
   ```

## Step 3: Run the Tool

1. Still in the x64 Native Tools Command Prompt:
   ```cmd
   run_tool_x64.bat notepad.exe
   ```

2. This will:
   - Start Task Manager
   - Inject the DLL into Task Manager
   - Start Notepad
   - Hide Notepad from Task Manager

## Troubleshooting

If you still see the 16-bit application error:
- Make sure you're using the x64 Native Tools Command Prompt, not the regular Command Prompt
- Make sure you built with build_x64.bat, not the regular build.bat
- Check that both win_process_hider.dll and win_injector.exe were created

## Fixing "Unsupported 16-Bit Application" Error

This error occurs because Windows 11 64-bit cannot run 16-bit or incorrectly compiled 32-bit applications. To fix:

1. **Always use the x64 Native Tools Command Prompt** (not the regular Developer Command Prompt)
2. Compile with the `/MACHINE:X64` flag as specified in the build_x64.bat script
3. Make sure to run as Administrator

The build_x64.bat script has been specifically modified to create proper 64-bit binaries that will work on Windows 11.
