@echo off
REM Build script for Windows Process Hiding Tool

echo ===============================================================
echo Building Windows Process Hiding Tool - EDUCATIONAL PURPOSE ONLY
echo ===============================================================
echo.

REM Check for compiler
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Microsoft Visual C++ compiler (cl.exe) not found.
    echo Please run this from a Visual Studio Developer Command Prompt
    echo or run vcvarsall.bat to set up the environment.
    goto end
)

echo Building DLL (win_process_hider.dll)...
cl /LD win_process_hider.c /link /out:win_process_hider.dll kernel32.lib user32.lib psapi.lib

echo Building injector (win_injector.exe)...
cl win_injector.c /link /out:win_injector.exe kernel32.lib user32.lib

if exist win_process_hider.dll (
    echo.
    echo Build successful!
    echo.
    echo Usage:
    echo   hide_process.bat ^<process_to_hide^>
    echo   Example: hide_process.bat notepad.exe
) else (
    echo.
    echo Build failed!
)

:end
