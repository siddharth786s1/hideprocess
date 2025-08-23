:: Windows Process Hiding Tool
:: Run this batch file to quickly hide a process

@echo off
@setlocal

:: Check arguments
if "%~1"=="" (
    echo Usage: hideproc.bat [process_name.exe]
    echo Example: hideproc.bat notepad.exe
    goto end
)

:: Check if the tool is installed
if not exist "win_injector.exe" (
    echo Error: Required files not found.
    echo Please run installer.bat first.
    goto end
)

:: Run the process hider
echo Hiding process: %~1
call hide_process.bat %~1

:end
endlocal
