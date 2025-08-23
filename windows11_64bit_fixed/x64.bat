@echo off
REM Quick x64 architecture switch - USE THIS FIRST!

echo ===============================================================
echo Windows 64-bit Architecture Switcher
echo ===============================================================
echo.

if defined VSINSTALLDIR (
    echo Current architecture: %VSCMD_ARG_TGT_ARCH%
    
    if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" (
        echo You are already using x64 architecture. Good!
    ) else (
        echo Switching to x64 architecture...
        echo.
        call vcvarsall.bat x64
        
        if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" (
            echo [SUCCESS] Switched to x64 architecture!
        ) else (
            echo [ERROR] Failed to switch to x64 architecture.
            echo Please use the x64 Native Tools Command Prompt instead.
        )
    )
) else (
    echo [ERROR] This is not a Visual Studio Command Prompt.
    echo.
    echo Please run this from a Visual Studio Command Prompt:
    echo 1. Search for "Developer Command Prompt" in Start menu
    echo 2. Right-click and "Run as administrator" 
    echo 3. Run this script again in that command prompt
)

echo.
echo Current architecture: %VSCMD_ARG_TGT_ARCH%
echo.
echo ===============================================================

pause
