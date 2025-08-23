# Windows Process Hiding Tool - Setup Guide

## Requirements

- Windows 10 or Windows 11
- Visual Studio with C/C++ development tools installed OR Visual C++ Build Tools
- Administrator privileges

## Installation Steps

1. Copy all files to a directory on your Windows system.

2. Open a **Visual Studio Developer Command Prompt** as **Administrator**:
   - Search for "Developer Command Prompt for VS" in the Start menu
   - Right-click on it and select "Run as administrator"

3. Navigate to the directory containing the project files:
   ```cmd
   cd path\to\hideprocess
   ```

4. Build the tool by running the build script:
   ```cmd
   build.bat
   ```
   This will compile both the DLL and the injector.

5. If successful, you should see a message indicating the build was completed.

## Usage Instructions

1. Open Task Manager or another process monitoring tool that you want to test with.

2. In the same **Administrator Command Prompt**, run the hiding tool:
   ```cmd
   hide_process.bat <process_name_to_hide>
   ```
   For example:
   ```cmd
   hide_process.bat notepad.exe
   ```

3. Start the process you specified (if it's not already running).

4. The process should now be hidden from Task Manager and other monitoring tools that were running when you executed the script.

## Troubleshooting

If you encounter issues:

1. **Build errors**:
   - Make sure you're using a Visual Studio Developer Command Prompt
   - Check that you have the required C/C++ development tools installed

2. **Process not hiding**:
   - Ensure you're running both the build and hiding scripts as Administrator
   - Make sure at least one monitoring tool (e.g., Task Manager) is running before you run the hiding tool
   - Try specifying the full process name including the .exe extension

3. **Access denied errors**:
   - Some process monitoring tools have protection mechanisms that prevent DLL injection
   - Try with the standard Windows Task Manager first

## Educational Notes

This tool demonstrates:
- Windows API hooking techniques
- DLL injection methods
- Process enumeration mechanisms
- Security concepts related to process visibility

Remember that this tool is created for **educational purposes only**. Understanding these techniques helps security professionals develop better detection methods and protections against malware that uses similar techniques.
