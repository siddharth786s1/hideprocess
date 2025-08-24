@echo off
echo ================================================================
echo COMPREHENSIVE TEST SUITE - Process Hiding Tool
echo ================================================================
echo.

echo 1. Testing build environment...
if not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo ❌ ERROR: Not in x64 environment
    echo Please run from "x64 Native Tools Command Prompt for VS 2022"
    exit /b 1
) else (
    echo ✅ x64 environment detected
)

echo.
echo 2. Building the project...
call build_x64.bat
if %errorlevel% neq 0 (
    echo ❌ Build failed!
    exit /b 1
) else (
    echo ✅ Build successful
)

echo.
echo 3. Checking if notepad is running...
tasklist /FI "IMAGENAME eq notepad.exe" | find /I "notepad.exe" >nul
if %errorlevel% neq 0 (
    echo 📝 Starting notepad.exe for testing...
    start notepad.exe
    timeout /t 2 /nobreak >nul
)

echo.
echo 4. Verifying Task Manager is running...
tasklist /FI "IMAGENAME eq taskmgr.exe" | find /I "taskmgr.exe" >nul
if %errorlevel% neq 0 (
    echo 📊 Starting Task Manager...
    start taskmgr.exe
    timeout /t 3 /nobreak >nul
)

echo.
echo 5. Running process hiding tool...
win_injector.exe notepad.exe

echo.
echo 6. Test complete! 
echo 📋 Check Task Manager to see if notepad.exe is hidden
echo 📝 Check C:\injection_log.txt for detailed logs
echo.
pause
