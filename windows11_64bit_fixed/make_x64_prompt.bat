@echo off
REM This script converts any Visual Studio command prompt to use x64 architecture

echo ===============================================================
echo Visual Studio Command Prompt Architecture Converter
echo ===============================================================
echo.

REM Check if we're in a Visual Studio command prompt
if not defined VSINSTALLDIR (
    echo ERROR: This is not a Visual Studio command prompt.
    echo.
    echo Please run this script from any Visual Studio command prompt:
    echo 1. Search for "Developer Command Prompt" in the Start menu
    echo 2. Open it, then run this script again
    goto end
)

REM Check current architecture
echo Current architecture target: %VSCMD_ARG_TGT_ARCH%

REM Switch to x64 if needed
if /I not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo.
    echo Converting command prompt to x64 architecture...
    
    REM Find vcvarsall.bat
    if exist "%VSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" (
        call "%VSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" x64
    ) else if exist "%VSINSTALLDIR%\VC\vcvarsall.bat" (
        call "%VSINSTALLDIR%\VC\vcvarsall.bat" x64
    ) else (
        echo [ERROR] Could not find vcvarsall.bat
        echo Please use the dedicated x64 Native Tools Command Prompt instead.
        goto end
    )
    
    REM Verify the conversion worked
    if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" (
        echo [SUCCESS] Successfully converted to x64 architecture!
        echo Current architecture target: x64
    ) else (
        echo [ERROR] Failed to convert to x64 architecture.
        echo Please use the dedicated x64 Native Tools Command Prompt instead.
        goto end
    )
) else (
    echo.
    echo [OK] Already using x64 architecture target.
)

echo.
echo ===============================================================
echo Your command prompt is now configured for 64-bit development!
echo ===============================================================
echo.
echo You can now proceed with:
echo   verify_64bit.bat  - to verify your environment
echo   build_x64.bat     - to build the 64-bit version
echo   run_tool_x64.bat  - to run the tool
echo.

:end
pause
