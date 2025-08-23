@echo off
REM Run script for Windows Process Hiding Tool

echo ===============================================================
echo Running Windows Process Hiding Tool - EDUCATIONAL PURPOSE ONLY
echo ===============================================================
echo.

REM Check if files exist
if not exist win_process_hider.dll (
    echo ERROR: win_process_hider.dll not found.
    echo Please build first using build.bat
    goto end
)

if not exist win_injector.exe (
    echo ERROR: win_injector.exe not found.
    echo Please build first using build.bat
    goto end
)

REM Make sure Task Manager is running
echo Starting Task Manager...
start taskmgr

REM Wait a few seconds
echo Waiting for Task Manager to start...
timeout /t 3

REM Run the hiding tool
echo Running process hiding tool...
hide_simple.bat notepad.exe

REM Start notepad to test
echo Starting notepad for testing...
start notepad

echo.
echo Check Task Manager - notepad.exe should be hidden!

:end
pause
