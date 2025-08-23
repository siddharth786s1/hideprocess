@echo off
:: Troubleshooting script for Windows Process Hiding Tool

echo ===============================================================
echo Windows Process Hiding Tool - Troubleshooting
echo ===============================================================
echo.

echo Checking for required files...
if exist win_process_hider.c (
    echo [OK] win_process_hider.c found
) else (
    echo [MISSING] win_process_hider.c not found
)

if exist win_injector.c (
    echo [OK] win_injector.c found
) else (
    echo [MISSING] win_injector.c not found
)

if exist win_process_hider.dll (
    echo [OK] win_process_hider.dll found
) else (
    echo [MISSING] win_process_hider.dll not found
)

if exist win_injector.exe (
    echo [OK] win_injector.exe found
) else (
    echo [MISSING] win_injector.exe not found
)

echo.
echo Checking Visual C++ compiler...
where cl >nul 2>&1
if %errorlevel% equ 0 (
    echo [OK] C++ compiler found
) else (
    echo [MISSING] C++ compiler not found
    echo You need to run this from a Visual Studio Developer Command Prompt
)

echo.
echo Checking for running monitoring tools...
tasklist /fi "imagename eq taskmgr.exe" | find /i "taskmgr.exe" >nul
if %errorlevel% equ 0 (
    echo [OK] Task Manager is running
) else (
    echo [NOT RUNNING] Task Manager is not running
)

echo.
echo Recommended steps:
echo 1. Make sure you're running as Administrator
echo 2. Make sure you're in a Visual Studio Developer Command Prompt
echo 3. Run build.bat to compile the code
echo 4. Start Task Manager (taskmgr.exe)
echo 5. Run hide_process.bat notepad.exe
echo.

pause
