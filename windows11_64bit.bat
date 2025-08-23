@echo off
REM All-in-one script for Windows 11 64-bit Process Hiding Tool

echo ===============================================================
echo Windows 11 64-bit Process Hiding Tool - ALL-IN-ONE LAUNCHER
echo ===============================================================
echo.

REM Process to hide (default to notepad.exe if not provided)
if "%1"=="" (
    set PROCESS_TO_HIDE=notepad.exe
    echo No process specified, defaulting to: notepad.exe
    echo.
    echo To hide a different process, run: %0 process_name.exe
    echo.
) else (
    set PROCESS_TO_HIDE=%1
)

REM Check if we're running as administrator
echo Checking administrator privileges...
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: This script requires administrator privileges.
    echo.
    echo Please right-click on the Command Prompt or PowerShell and select
    echo "Run as administrator", then try again.
    goto end
)

REM Check for x64 environment
echo Checking for 64-bit environment...
if not defined VSCMD_ARG_TGT_ARCH (
    echo WARNING: This does not appear to be a Visual Studio Command Prompt.
    echo Results may vary.
    echo.
)

REM Build the tools if needed
echo Checking for compiled tools...
if not exist win_process_hider.dll (
    echo Need to build the tools first.
    echo.
    
    REM Check for compiler
    where cl >nul 2>&1
    if %errorlevel% neq 0 (
        echo ERROR: Microsoft Visual C++ compiler (cl.exe) not found.
        echo.
        echo Please run this from a Visual Studio x64 Native Tools Command Prompt:
        echo 1. Search for "x64 Native" in the Start menu
        echo 2. Right-click on "x64 Native Tools Command Prompt for VS"
        echo 3. Select "Run as administrator"
        echo 4. Navigate to this directory and try again
        goto end
    )
    
    echo Building 64-bit tools...
    echo.
    
    echo Building DLL (win_process_hider.dll)...
    cl /LD /MACHINE:X64 win_process_hider_x64.c /link /out:win_process_hider.dll kernel32.lib user32.lib psapi.lib
    if %errorlevel% neq 0 goto build_failed
    
    echo Building injector (win_injector.exe)...
    cl /MACHINE:X64 win_injector_x64.c /link /out:win_injector.exe kernel32.lib user32.lib
    if %errorlevel% neq 0 goto build_failed
    
    echo.
    echo Build completed successfully!
    echo.
) else (
    echo Found existing compiled tools.
)

REM Make sure Task Manager is running
echo Starting Task Manager...
start taskmgr

REM Wait a few seconds
echo Waiting for Task Manager to initialize...
timeout /t 5 >nul

REM Run the hiding tool
echo Running process hiding tool for %PROCESS_TO_HIDE%...
win_injector.exe %PROCESS_TO_HIDE%
if %errorlevel% neq 0 goto run_failed

REM Start the target process to test
echo Starting %PROCESS_TO_HIDE% for testing...
start %PROCESS_TO_HIDE%

echo.
echo SUCCESS! Check Task Manager - %PROCESS_TO_HIDE% should be hidden!
echo.
echo NOTE: If you don't see Task Manager, it may be minimized to the taskbar.
echo.
echo IMPORTANT: You will need to restart Task Manager to see hidden processes again.
goto end

:build_failed
echo.
echo ERROR: Build failed!
echo.
echo Please make sure you're running from an x64 Native Tools Command Prompt.
goto end

:run_failed
echo.
echo ERROR: Failed to run the process hiding tool!
echo.
echo Please check the output above for error messages.
goto end

:end
echo.
echo ===============================================================
pause
