@echo off
REM Build script for Windows Process Hiding Tool (64-bit)

echo ===============================================================
echo Building Windows Process Hiding Tool (64-bit) - EDUCATIONAL PURPOSE ONLY
echo ===============================================================
echo.

REM Check if we're running in the correct environment
echo Checking environment...
if not defined VSCMD_ARG_TGT_ARCH (
    echo ERROR: This script must be run from an x64 Native Tools Command Prompt.
    echo Please follow these steps:
    echo 1. Close this window
    echo 2. Search for "x64 Native Tools Command Prompt for VS" in Start menu
    echo 3. Right-click and select "Run as administrator"
    echo 4. Navigate to this directory and try again
    goto end
)

if /I not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo ERROR: This is not an x64 build environment.
    echo Please use the x64 Native Tools Command Prompt.
    goto end
)

REM Check for compiler
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Microsoft Visual C++ compiler (cl.exe) not found.
    echo Please run this from an x64 Native Tools Command Prompt
    echo or run vcvarsall.bat with x64 architecture.
    goto end
)

echo Building DLL (win_process_hider.dll) for 64-bit...
cl /LD /MACHINE:X64 win_process_hider_x64.c /link /out:win_process_hider.dll kernel32.lib user32.lib psapi.lib

echo Building injector (win_injector.exe) for 64-bit...
cl /MACHINE:X64 win_injector_x64.c /link /out:win_injector.exe kernel32.lib user32.lib

if exist win_process_hider.dll (
    echo.
    echo Build successful!
    echo.
    echo Usage:
    echo   run_tool_x64.bat ^<process_to_hide^>
    echo   Example: run_tool_x64.bat notepad.exe
) else (
    echo.
    echo Build failed!
)

:end
