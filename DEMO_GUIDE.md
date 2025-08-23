# Windows Process Hiding Tool - Demo Script

This script will guide you through a complete demonstration of the process hiding tool.

## Prerequisites

- Windows 10/11 system
- Visual Studio Build Tools installed
- Administrator privileges

## Demonstration Steps

### 1. Build the Tool

1. Open an **Administrator Command Prompt** by right-clicking Command Prompt and selecting "Run as administrator"

2. Navigate to the directory containing the extracted files:
   ```cmd
   cd path\to\hideprocess
   ```

3. Run the build script:
   ```cmd
   build.bat
   ```

4. You should see output indicating successful compilation of:
   - win_process_hider.dll
   - win_injector.exe

### 2. Set Up Test Environment

1. Launch Notepad:
   ```cmd
   start notepad
   ```

2. Open Task Manager:
   ```cmd
   taskmgr
   ```

3. In Task Manager, click on "More details" if it's in simple view
   
4. Go to the "Details" tab to see all running processes
   
5. Verify that "notepad.exe" is visible in the list of processes

### 3. Hide a Process

1. In the Administrator Command Prompt, run:
   ```cmd
   hide_process.bat notepad.exe
   ```

2. The script will:
   - Check if you're running as Administrator
   - Detect running monitoring tools
   - Inject the DLL into Task Manager
   - Hide all instances of notepad.exe

### 4. Verify the Results

1. Look at Task Manager again
   - Notepad.exe should no longer appear in the process list
   - The Notepad window should still be running and visible on your desktop

2. Try using other system monitoring tools like Process Explorer (if installed)
   - The process should be hidden from these tools as well

### 5. Additional Tests

1. Try hiding other processes:
   ```cmd
   hide_process.bat calculator.exe
   ```

2. Start the process after hiding:
   ```cmd
   start calc
   ```
   - Notice that it doesn't appear in Task Manager even though it's running

3. Try closing and reopening Task Manager
   - You'll need to run the hide_process.bat command again as the hooks are lost when Task Manager restarts

## Educational Points

- The process is still running even though it's not visible in monitoring tools
- This demonstrates how malware could potentially hide itself
- Modern security tools use multiple detection methods to find hidden processes
- Understanding these techniques helps in building better security solutions
