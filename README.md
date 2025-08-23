# Windows Process Hiding Tool (Educational)

> **⚠️ IMPORTANT DISCLAIMER ⚠️**  
> This tool is provided strictly for educational and security research purposes only. Unauthorized use to hide malicious processes is illegal and unethical. Always use this code in controlled environments with proper authorization.

## Overview

This project demonstrates how process hiding works on Windows by hooking relevant Windows API calls. Understanding these techniques is valuable for security professionals to improve detection mechanisms.

## Project Structure

The project contains the following files:
1. `win_process_hider.c` - DLL that hooks process listing APIs
2. `win_injector.c` - Utility to inject our DLL into target processes
3. `hide_process.bat` - Helper script for easy usage
4. `build.bat` - Build script

## Compilation

To compile the code, you need a Microsoft Visual C++ compiler. Follow these steps:

1. Open a Visual Studio Developer Command Prompt
2. Navigate to the project directory
3. Run the build script:
   ```
   build.bat
   ```

## Usage Instructions

1. Make sure you have the compiled DLL (`win_process_hider.dll`) and executable (`win_injector.exe`) in the same directory
2. Open a Command Prompt as Administrator
3. Run the tool using the helper script:
   ```
   hide_process.bat <process_name_to_hide>
   ```
   For example: `hide_process.bat notepad.exe`

4. The specified process will be hidden from Task Manager and other supported monitoring tools

## How It Works

### DLL (win_process_hider.c)
The DLL uses API hooking to intercept calls to process enumeration functions. When these functions are called, our hooks filter out the target process from the results.

Key functions that are hooked:
- `Process32First` - Used by tools to get the first process in a snapshot
- `Process32Next` - Used to iterate through processes
- `EnumProcesses` - Used to enumerate all running processes

### Injector (win_injector.c)
The injector loads our DLL into monitoring tools using the CreateRemoteThread technique with the following steps:
1. Find the target monitoring tools by name (Task Manager, Process Explorer, etc.)
2. Open the target process with appropriate permissions
3. Allocate memory in the target process
4. Write the DLL path to the allocated memory
5. Create a remote thread that calls LoadLibraryA with our DLL path
6. The DLL is loaded and hooks are installed

Environment variables are used to pass the target process name to the DLL.

## Limitations

- This educational tool uses basic API hooking techniques
- Some advanced monitoring tools may detect the injection
- Requires administrator privileges
- Only hides by process name, not by PID or other criteria
- Works best with 32-bit processes due to simplified hooking approach
- May not work with all process monitoring utilities

## Ethical Considerations

This tool is strictly for educational purposes to understand:
- How Windows process management works
- API hooking techniques
- DLL injection methods
- Security limitations of process monitoring tools

Unauthorized use to hide malicious processes is illegal and unethical.