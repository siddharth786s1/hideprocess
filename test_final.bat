@echo off
echo ==========================================
echo FINAL PROCESS HIDING TEST - CLEAN VERSION
echo ==========================================

echo Step 1: Starting notepad for testing...
start notepad.exe
timeout /t 2 /nobreak >nul

echo Step 2: Verify notepad is running...
tasklist | findstr /i notepad
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: notepad.exe is not running!
    pause
    exit /b 1
)

echo [SUCCESS] notepad.exe is running

echo.
echo Step 3: Running injection...
win_injector.exe
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Injection failed
    pause
    exit /b 1
)

echo.
echo Step 4: Testing hiding effectiveness...
echo.
echo [INFO] Please open Task Manager now and check:
echo   1. Is notepad.exe VISIBLE in the process list?
echo   2. After injection, is notepad.exe HIDDEN?
echo.
echo If notepad disappears from Task Manager after injection,
echo the process hiding is working correctly!
echo.

echo Step 5: Check logs...
if exist C:\process_hider_log.txt (
    echo [FOUND] Process hider log:
    type C:\process_hider_log.txt
) else (
    echo [WARNING] No process hider log found
)

if exist C:\injection_log.txt (
    echo.
    echo [FOUND] Injection log:
    type C:\injection_log.txt
) else (
    echo [WARNING] No injection log found
)

echo.
echo Test completed! Check Task Manager to verify hiding.
pause
