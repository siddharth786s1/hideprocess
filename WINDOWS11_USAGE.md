# Windows 11 64-bit Usage Guide

This guide provides instructions for using the Process Hiding Tool specifically on Windows 11 64-bit systems.

## IMPORTANT: Fixing "Unsupported 16-Bit Application" Error

If you see the error message "The program or feature cannot start or run due to incompatibility with 64-bit versions of Windows", follow these specific steps:

1. You **MUST** use the **x64 Native Tools Command Prompt for VS 2022** (not regular Command Prompt)
2. You **MUST** run as Administrator
3. You **MUST** use the build_x64.bat script which properly flags the binaries for 64-bit

See the WINDOWS11_64BIT_GUIDE.md file for complete instructions to fix this error.

## Prerequisites

1. Windows 11 64-bit (or Windows 10 64-bit)
2. Administrator privileges
3. Visual Studio Build Tools (for compilation)

## Setup

1. Extract all files to a folder on your Windows 11 system
2. Run `check_64bit.bat` to verify your system compatibility
3. Open a Visual Studio x64 Native Tools Command Prompt as Administrator
4. Navigate to the folder containing the extracted files

## Building the Tool

1. In the command prompt, run:
   ```
   build_x64.bat
   ```

2. This will compile the 64-bit versions of:
   - `win_process_hider.dll`
   - `win_injector.exe`

## Using the Tool

1. To hide a process (e.g., notepad.exe), run:
   ```
   run_tool_x64.bat notepad.exe
   ```

2. The script will:
   - Check for administrator privileges
   - Start Task Manager
   - Inject the DLL to hide the specified process
   - Start an instance of the target process

3. Verify that the process is hidden in Task Manager

## Troubleshooting

If you encounter any issues:

1. Run `check_64bit.bat` to verify your system compatibility
2. Run `troubleshoot.bat` to check for common issues
3. Make sure you're using the 64-bit build environment
4. Ensure you have administrator privileges
5. Try temporarily disabling antivirus software (it may detect the tool as malicious)

## Common Issues on Windows 11

1. **"16-bit MS-DOS Subsystem" Error**:
   - This indicates you're trying to run a 16-bit application on 64-bit Windows
   - Make sure to use the 64-bit versions of all files
   - Compile using build_x64.bat in an x64 Native Tools Command Prompt

2. **Access Denied Error**:
   - Run all commands as Administrator
   - Check Windows Defender or other security software

3. **DLL Not Found**:
   - Make sure the DLL was compiled successfully
   - Check that it's in the same directory as the injector

## Educational Note

This tool is provided for educational purposes only. Use responsibly and only on systems you own or have permission to test.
