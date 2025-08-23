# Windows 11 64-bit Process Hiding Tool - SIMPLE GUIDE

## IMPORTANT: READ THIS FIRST

This tool requires:
- Windows 11 64-bit (or Windows 10 64-bit)
- Visual Studio with C++ development tools
- Administrator privileges

If you've been getting the "16-bit application" error, this is the solution!

## ONE-STEP SOLUTION

1. Open an **x64 Native Tools Command Prompt for VS 2022** as Administrator:
   - Search for "x64 Native" in the Start menu
   - Right-click on "x64 Native Tools Command Prompt for VS 2022"
   - Select "Run as administrator"

2. Navigate to your folder:
   ```
   cd path\to\your\folder
   ```

3. Run the all-in-one script:
   ```
   windows11_64bit.bat notepad.exe
   ```

This script will:
- Check if you have administrator privileges
- Build the tool if needed
- Start Task Manager
- Hide the specified process (notepad.exe in this example)
- Start a test instance of the process

## Why This Works When Other Solutions Failed

1. It uses the correct 64-bit source files
2. It compiles with the `/MACHINE:X64` flag
3. It runs from the x64 Native Tools Command Prompt

## Troubleshooting

If you still have issues:

1. Make sure you're using the **x64 Native Tools Command Prompt for VS 2022**
2. Make sure you're running as Administrator
3. If you get compilation errors, install Visual Studio Build Tools with C++ support
4. Check that both win_process_hider.dll and win_injector.exe are created

## Educational Purpose Only

This tool is provided for educational purposes only. Use responsibly and only on systems you own or have permission to test.
