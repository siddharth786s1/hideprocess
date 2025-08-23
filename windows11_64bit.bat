@echo off
REM Windows 11 64-bit batch file

echo ===============================================================
echo Windows 11 64-bit Process Hiding Tool - EDUCATIONAL PURPOSE ONLY
echo ===============================================================
echo.

REM Check if we're on Windows 11
for /f "tokens=4-5 delims=[.] " %%i in ('ver') do set VERSION=%%i.%%j
if not "%VERSION%" GEQ "10.0" (
    echo ERROR: This tool requires Windows 10 or newer.
    goto end
)

REM Check if we're on 64-bit Windows
reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set OS=32BIT || set OS=64BIT
if %OS%==32BIT (
    echo ERROR: This tool requires 64-bit Windows.
    goto end
)

REM Check if we're running as Administrator
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: This script requires administrator privileges.
    echo Please right-click and select "Run as administrator"
    goto end
)

REM Check if we're in the correct command prompt
if not defined VSCMD_ARG_TGT_ARCH (
    echo ERROR: This script must be run from an x64 Native Tools Command Prompt.
    echo Please follow these steps:
    echo 1. Search for "x64 Native Tools Command Prompt for VS" in Start menu
    echo 2. Right-click and select "Run as administrator"
    echo 3. Navigate to this directory and try again
    goto end
)

if /I not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo ERROR: This is not an x64 build environment.
    echo Please use the x64 Native Tools Command Prompt.
    goto end
)

REM Check if files exist and build if needed
if not exist win_process_hider.dll (
    echo DLL not found. Building...
    call build_x64.bat
    if not exist win_process_hider.dll (
        echo ERROR: Build failed. Cannot continue.
        goto end
    )
)

if not exist win_injector.exe (
    echo Injector not found. Building...
    call build_x64.bat
    if not exist win_injector.exe (
        echo ERROR: Build failed. Cannot continue.
        goto end
    )
)

echo Everything looks good! Your system is correctly set up for the tool.
echo.
echo To hide a process, use:
echo   run_tool_x64.bat ^<process_name^>
echo   Example: run_tool_x64.bat notepad.exe

:end
pause
