# Windows 11 64-bit Process Hiding Tool

## HOW TO FIX "NOT USING X64 ARCHITECTURE TARGET" ERROR

You are seeing this error because you're using the wrong command prompt. Follow these exact steps:

### Method 1 (RECOMMENDED): Use the x64 Native Tools Command Prompt

1. Close your current command prompt
2. Click on Windows Start menu
3. Type "x64 Native" in search
4. Look for "x64 Native Tools Command Prompt for VS 2022" 
5. RIGHT-CLICK on it and select "Run as administrator"
6. Navigate to your folder:
   ```
   cd C:\path\to\folder
   ```
7. Run the verification tool:
   ```
   verify_64bit.bat
   ```
   It should show "Current architecture target: x64"

8. Build the tool:
   ```
   build_x64.bat
   ```

9. Run the tool:
   ```
   run_tool_x64.bat notepad.exe
   ```

### Method 2: Convert Your Current Command Prompt

If you can't find the x64 Native Tools Command Prompt:

1. Open any Visual Studio Developer Command Prompt as Administrator
2. Navigate to your folder:
   ```
   cd C:\path\to\folder
   ```
3. Run the converter:
   ```
   x64.bat
   ```
4. Check that it worked:
   ```
   echo %VSCMD_ARG_TGT_ARCH%
   ```
   It should show "x64"

5. Continue with verification and building:
   ```
   verify_64bit.bat
   build_x64.bat
   run_tool_x64.bat notepad.exe
   ```

## WHY YOU'RE GETTING THIS ERROR

Windows 11 is a 64-bit system that requires 64-bit applications. Your current command prompt is set to x86 architecture, which produces 32-bit applications that aren't compatible.

The key is to use a command prompt with x64 architecture target.
