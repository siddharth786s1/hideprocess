@echo off
echo Testing if notepad.exe is running...
tasklist /FI "IMAGENAME eq notepad.exe" | find /I "notepad.exe"
if %ERRORLEVEL%==0 (
    echo notepad.exe is running in the system
) else (
    echo notepad.exe is not running
    echo Starting notepad.exe for testing...
    start notepad.exe
    timeout /t 2 /nobreak
)

echo.
echo Checking again...
tasklist /FI "IMAGENAME eq notepad.exe" | find /I "notepad.exe"
if %ERRORLEVEL%==0 (
    echo notepad.exe is now running
) else (
    echo notepad.exe failed to start
)
