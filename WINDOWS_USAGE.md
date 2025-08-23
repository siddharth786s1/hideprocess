# Windows Process Hiding Tool - Usage Instructions

## Important: This Tool Must Be Used On Windows

This tool cannot be compiled or run in your Ubuntu container. You must transfer the files to a Windows system.

## Step-by-Step Windows Instructions

1. **Transfer these files to a Windows system:**
   - win_process_hider.c
   - win_injector.c
   - build.bat
   - hide_process.bat
   - hide_simple.bat
   - run_tool.bat

2. **On Windows, open a Visual Studio Developer Command Prompt as Administrator:**
   - Search for "Developer Command Prompt for VS" in the Start menu
   - Right-click and select "Run as Administrator"

3. **Navigate to the folder with your files:**
   ```cmd
   cd path\to\your\files
   ```

4. **Build the tool:**
   ```cmd
   build.bat
   ```

5. **Run the tool:**
   ```cmd
   run_tool.bat
   ```
   
   OR run the individual steps manually:
   ```cmd
   start taskmgr
   timeout /t 3
   hide_simple.bat notepad.exe
   start notepad
   ```

## Troubleshooting

If you see "win_process_hider.dll and win_injector.exe file not found":

1. Make sure you're on a Windows system (not Ubuntu)
2. Make sure you're using a Visual Studio Developer Command Prompt
3. Make sure the build completed successfully
4. Check that the .dll and .exe files exist in the same folder

## Installing Visual Studio Build Tools

If you don't have Visual Studio installed:

1. Download Visual Studio Build Tools from:
   https://visualstudio.microsoft.com/visual-cpp-build-tools/

2. Run the installer and select "C++ build tools" during installation

3. After installation, use "Developer Command Prompt for VS" from the Start menu

## Why This Doesn't Work in Ubuntu

The tool requires the Microsoft Visual C++ compiler (cl.exe) which only exists on Windows systems with Visual Studio installed. Your current environment is an Ubuntu container which cannot natively compile Windows executables.

## Educational Purpose

Remember that this tool is for educational purposes only. It demonstrates:
- Windows API hooking techniques
- DLL injection for process monitoring
- Security research methodologies
