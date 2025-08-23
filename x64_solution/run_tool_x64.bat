@echo off
REM Run script for Windows Process Hiding Tool (64-bit)

echo ===============================================================
echo Running Windows Process Hiding Tool (64-bit) - EDUCATIONAL PURPOSE ONLY
echo ===============================================================
echo.

if "%1"=="" (
    echo Usage: %0 ^<process_name_to_hide^>
    echo Example: %0 notepad.exe
    goto end
)

REM Check if files exist
if not exist win_process_hider.dll (
    echo ERROR: win_process_hider.dll not found.
    echo Please build first using build_x64.bat
    goto end
)

if not exist win_injector.exe (
    echo ERROR: win_injector.exe not found.
    echo Please build first using build_x64.bat
    goto end
)

REM Check if we're running as administrator
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: This script requires administrator privileges.
    echo Please right-click and select "Run as administrator"
    goto end
)

REM Make sure Task Manager is running
echo Starting Task Manager...
start taskmgr

REM Wait a few seconds
echo Waiting for Task Manager to start...
timeout /t 5

REM Run the hiding tool
echo Running process hiding tool for %1...
win_injector.exe %1

REM Start the target process to test
echo Starting %1 for testing...
start %1

echo.
echo Check Task Manager - %1 should be hidden!

:end
pause
