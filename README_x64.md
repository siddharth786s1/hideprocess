# Windows 64-bit Process Hiding Tool (Updated)

This tool is provided for **educational purposes** only to demonstrate how process hiding works in Windows systems through API hooking. It allows you to hide a specific process from task managers and other monitoring tools.

## Features

- Hides target processes from Task Manager, Process Explorer, and other monitoring tools
- 64-bit compatible for Windows 10/11
- Uses DLL injection and API hooking techniques
- Educational tool for understanding Windows API hooking

## Usage Instructions

1. Make sure you have Visual Studio build tools installed (specifically the C/C++ compiler)
2. Open an x64 Native Tools Command Prompt for Visual Studio
3. Navigate to the tool's directory
4. Build the 64-bit version using:
   ```
   build_x64.bat
   ```
5. Run the process hiding tool:
   ```
   hide_process_x64.bat notepad.exe
   ```
   Replace `notepad.exe` with the name of the process you want to hide.

## How It Works

The tool uses the following techniques:

1. **DLL Injection**: The injector injects our DLL into process monitoring tools
2. **API Hooking**: The DLL hooks Windows API functions related to process enumeration
3. **Process Filtering**: The hooked functions filter out the target process from results

## Technical Details

- **win_injector.c**: Injects the DLL into monitoring tools like Task Manager
- **win_process_hider.c**: Contains the hooking logic for process enumeration functions
- **build_x64.bat**: Compiles the tool for 64-bit Windows systems
- **hide_process_x64.bat**: Helper script to run the tool with the target process

## Requirements

- Windows 10/11 (64-bit)
- Visual Studio build tools (C/C++ compiler)
- Administrative privileges

## Educational Purposes Only

This tool is provided strictly for educational purposes to understand Windows internals, API hooking, and process management. Use responsibly and only on your own systems for learning purposes.

## Troubleshooting

If you encounter any issues:

1. Ensure you're using the x64 Native Tools Command Prompt for Visual Studio
2. Make sure you have administrative privileges
3. Check that Task Manager is running before executing the tool
4. Verify that the process you want to hide is running
