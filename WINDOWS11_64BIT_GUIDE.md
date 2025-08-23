# Windows 11 64-bit Process Hiding Tool - COMPLETE GUIDE

This comprehensive guide will help you correctly set up and use the Process Hiding Tool on Windows 11 64-bit systems.

## IMPORTANT: Fixing the "Unsupported 16-Bit Application" Error

This error occurs because your Windows 11 64-bit system cannot run 16-bit or incorrectly compiled 32-bit applications. Follow these specific steps to fix it:

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

## Step 2: Use the Correct Command Prompt

You **MUST** use the "x64 Native Tools Command Prompt for VS 2022" (**not** the regular Command Prompt):

1. Click the Start menu
2. Search for "x64 Native" 
3. Right-click on "x64 Native Tools Command Prompt for VS 2022"
4. Select "Run as administrator"

![Visual Studio x64 Command Prompt](https://docs.microsoft.com/en-us/cpp/build/media/vcpp-start-menu-native-tools.png)

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

## Step 4: Verify Your Environment

1. Run the verification tool to confirm your environment is correctly set up:
   ```cmd
   verify_64bit.bat
   ```

2. Make sure it passes ALL checks before continuing:
   - Confirms you're on 64-bit Windows
   - Confirms you're in the correct x64 command prompt
   - Confirms the compiler is generating 64-bit code
   - Confirms all necessary files are present

## Troubleshooting

If you still see the 16-bit application error:
- Make sure you're using the x64 Native Tools Command Prompt, not the regular Command Prompt
- Make sure you built with build_x64.bat, not the regular build.bat
- Check that both win_process_hider.dll and win_injector.exe were created
- Run verify_64bit.bat and fix any issues it reports

## Common Problems and Solutions

### 1. "Unsupported 16-Bit Application" Error
- **Solution:** Use ONLY the x64 Native Tools Command Prompt and the _x64 versions of all files
- Make sure to compile with `/MACHINE:X64` flag as specified in the build_x64.bat script

### 2. "Command Not Found" Error
- **Solution:** Make sure you're in the correct directory with the batch files

### 3. "DLL Not Found" Error
- **Solution:** Make sure you've built the tool with build_x64.bat first

### 4. "Access Denied" Error
- **Solution:** Make sure you ran the Command Prompt as Administrator

### 5. Task Manager Crashes
- **Solution:** Try using Process Explorer instead (download from Microsoft Sysinternals)

## Final Verification

To verify everything is working correctly:

1. Run `verify_64bit.bat` - Make sure it passes all checks
2. Run `build_x64.bat` - Make sure both files compile successfully
3. Run `run_tool_x64.bat notepad.exe` - Verify notepad.exe is hidden from Task Manager

## Educational Note

This tool is provided for educational purposes only. Use responsibly and only on systems you own or have permission to test.
