@echo off
REM 64-bit verification script

echo ===============================================================
echo Windows Process Hiding Tool - 64-bit Environment Verification
echo ===============================================================
echo.

REM Check Windows architecture
if defined ProgramFiles(x86) (
    echo [OK] Running on 64-bit Windows
) else (
    echo [ERROR] Not running on 64-bit Windows
    echo This tool is designed for 64-bit Windows systems.
    goto end
)

REM Check if we're running in a Visual Studio environment
echo.
echo Checking development environment...
if defined VSCMD_ARG_TGT_ARCH (
    echo [OK] Running in a Visual Studio Command Prompt
    echo Current architecture target: %VSCMD_ARG_TGT_ARCH%
    
    if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" (
        echo [OK] CORRECT! Using x64 architecture target
    ) else (
        echo [ERROR] WRONG ARCHITECTURE! Not using x64 architecture target
        echo Current architecture target: %VSCMD_ARG_TGT_ARCH%
        echo.
        echo Please use one of these methods to fix:
        echo 1. Run x64.bat to convert this prompt to x64
        echo 2. Close this window and open "x64 Native Tools Command Prompt for VS 2022" instead
        goto end
    )
) else (
    echo [ERROR] Not running in a Visual Studio Command Prompt
    echo You need a proper development environment with x64 architecture target.
    echo.
    echo Please run a Visual Studio Command Prompt as Administrator.
    goto end
)

REM Check for Visual C++ compiler
echo.
echo Checking for Visual C++ compiler...
where cl >nul 2>&1
if %errorlevel% equ 0 (
    echo [OK] Found Visual C++ compiler
) else (
    echo [ERROR] Visual C++ compiler not found
    echo Please run this script from a Visual Studio Developer Command Prompt.
    goto end
)

REM All checks passed
echo.
echo [SUCCESS] All verification checks passed!
echo Your environment is correctly set up for 64-bit development.
echo.
echo You can now run:
echo   build_x64.bat - to build the tool
echo   run_tool_x64.bat notepad.exe - to run the tool

:end
echo.
echo ===============================================================
pause
