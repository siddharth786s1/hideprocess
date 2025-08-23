@echo off
:: Simple batch file to check if a process is running
:: This can help users troubleshoot Task Manager detection issues

echo ===============================================================
echo Process Checker Tool
echo ===============================================================
echo.

if "%1"=="" (
    echo Usage: check_process.bat ^<process_name^>
    echo Example: check_process.bat notepad.exe
    goto end
)

echo Checking if process "%1" is running...
echo.

tasklist /FI "IMAGENAME eq %1" 2>nul | find /I "%1" >nul
if %errorlevel% equ 0 (
    echo FOUND: Process "%1" is currently running.
) else (
    echo NOT FOUND: Process "%1" is not running.
)

echo.
echo If the process is running but not found in Task Manager,
echo it might be successfully hidden by the hiding tool.

:end
echo.
pause
