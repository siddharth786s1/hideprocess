# Windows 11 64-bit Process Hiding Tool

## FIXING THE "NOT USING X64 ARCHITECTURE TARGET" ERROR

Follow these steps to fix the error:

### STEP 1: Open the Correct Command Prompt

**Method A: Using the x64 Native Tools Command Prompt (RECOMMENDED)**
1. Click Start menu
2. Search for "x64 Native"
3. Right-click "x64 Native Tools Command Prompt for VS 2022" 
4. Select "Run as administrator"
5. Navigate to this folder: `cd C:\path\to\extracted\folder`
6. Skip to Step 3

**Method B: Using any Visual Studio Command Prompt**
1. Search for "Developer Command Prompt" in Start menu
2. Right-click and select "Run as administrator"
3. Navigate to this folder: `cd C:\path\to\extracted\folder`
4. Run the x64 switcher: `x64.bat`
5. Continue with Step 3 in the same window

### STEP 3: Verify Your Environment

Run:
```
verify_64bit.bat
```

It should show all [OK] messages and confirm you're using x64 architecture.

### STEP 4: Build the Tool

Run:
```
build_x64.bat
```

### STEP 5: Run the Tool

Run:
```
run_tool_x64.bat notepad.exe
```

## STILL HAVING TROUBLE?

If you're still seeing "not using x64 architecture target" errors:
1. Make sure you're using the x64 Native Tools Command Prompt
2. Try uninstalling and reinstalling Visual Studio with C++ components

## WHY THIS MATTERS

Windows 11 is a 64-bit operating system that requires 64-bit applications. When you compile with x86 architecture target, you get the "16-bit application" error because Windows is trying to run a 32-bit application in compatibility mode.
