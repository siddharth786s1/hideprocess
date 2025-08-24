@echo off
echo ===============================================================
echo COMPREHENSIVE TEST SUITE - Process Hiding Tool
echo ===============================================================

echo.
echo 1. Testing build environment...
if not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo ❌ ERROR: Not in x64 environment
    echo Please run from "x64 Native Tools Command Prompt for VS 2022"
    pause
    exit /b 1
)
echo ✅ x64 environment detected

echo.
echo 2. Building the project...
call build_x64.bat
if %errorlevel% neq 0 (
    echo ❌ Build failed!
    pause
    exit /b 1
)

echo.
echo 3. Starting notepad.exe for testing...
start notepad.exe
timeout /t 3

echo.
echo 4. Starting Task Manager...
start taskmgr.exe
timeout /t 5

echo.
echo 5. Running the injection...
win_injector.exe notepad.exe

echo.
echo ===============================================================
echo TEST COMPLETED
echo ===============================================================
echo.
echo Next steps:
echo 1. Check Task Manager - notepad.exe should be hidden
echo 2. Check C:\injection_log.txt for detailed logs
echo 3. Check C:\process_hider_debug.txt for DLL debug info
echo.
pause
