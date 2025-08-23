# How to Fix "Not Using x64 Architecture Target" Error

This guide will help you fix the error where your command prompt is using x86 architecture instead of x64.

## Solution 1: Use the x64 Native Tools Command Prompt (Recommended)

1. Close your current command prompt window
2. Click on the Windows Start menu
3. Search for "**x64 Native**"
4. Look for "**x64 Native Tools Command Prompt for VS 2022**" (or your Visual Studio version)
5. **RIGHT-CLICK** on it and select "**Run as administrator**"

![x64 Native Tools Command Prompt](https://i.imgur.com/zIwgRWt.png)

6. Navigate to your folder:
```
cd C:\path\to\hideprocess
```

7. Run the verification tool:
```
verify_64bit.bat
```

8. If successful, build and run the tool:
```
build_x64.bat
run_tool_x64.bat notepad.exe
```

## Solution 2: Convert Your Current Command Prompt to x64

If you can't find the x64 Native Tools Command Prompt, you can convert your current one:

1. Run the included conversion script:
```
make_x64_prompt.bat
```

2. After conversion, verify it worked:
```
echo %VSCMD_ARG_TGT_ARCH%
```
   It should output: `x64`

3. Run the verification tool:
```
verify_64bit.bat
```

4. If successful, build and run the tool:
```
build_x64.bat
run_tool_x64.bat notepad.exe
```

## How to Confirm You're Using x64 Architecture

When the command prompt is correctly set to x64:
- Running `echo %VSCMD_ARG_TGT_ARCH%` will output `x64`
- The verify_64bit.bat script will show all [OK] checks passed
- You won't get the 16-bit application error when running your tool

## What If I Don't Have Visual Studio Installed?

You need to install Visual Studio or Visual Studio Build Tools:

1. Download Visual Studio Build Tools:
   https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022

2. Run the installer
3. Select "Desktop development with C++"
4. Complete the installation
5. Follow Solution 1 above

## Troubleshooting

If you're still having issues:
1. Make sure you're running as Administrator
2. Try repairing your Visual Studio installation
3. Make sure you have the C++ development components installed
