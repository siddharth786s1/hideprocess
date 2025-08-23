@echo off
REM Build script for Windows Process Hiding Tool (64-bit)

echo ===============================================================
echo Building Windows Process Hiding Tool (64-bit) - EDUCATIONAL PURPOSE ONLY
echo ===============================================================
echo.

REM Check for compiler
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Microsoft Visual C++ compiler (cl.exe) not found.
    echo Please run this from a Visual Studio x64 Native Tools Command Prompt
    echo or run vcvarsall.bat with x64 architecture.
    goto end
)

echo Building DLL (win_process_hider.dll) for 64-bit...
cl /LD /MACHINE:X64 win_process_hider.c /link /out:win_process_hider.dll kernel32.lib user32.lib psapi.lib

echo Building injector (win_injector.exe) for 64-bit...
cl /MACHINE:X64 win_injector.c /link /out:win_injector.exe kernel32.lib user32.lib

if exist win_process_hider.dll (
    echo.
    echo Build successful!
    echo.
    echo Usage:
    echo   run_tool_x64.bat ^<process_to_hide^>
    echo   Example: run_tool_x64.bat notepad.exe
) else (
    echo.
    echo Build failed!
)

:end
