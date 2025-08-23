@echo off
REM 64-bit compatibility check script

echo ===============================================================
echo Windows Process Hiding Tool - 64-bit Compatibility Check
echo ===============================================================
echo.

REM Check Windows architecture
reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set OS=32BIT || set OS=64BIT

if %OS%==32BIT (
    echo WARNING: You are running a 32-bit version of Windows.
    echo This tool is optimized for 64-bit Windows systems.
    echo You may encounter compatibility issues.
) else (
    echo You are running a 64-bit version of Windows. Good!
)

echo.

REM Check Windows version
for /f "tokens=4-5 delims=[.] " %%i in ('ver') do set VERSION=%%i.%%j

echo Your Windows version: %VERSION%

if "%VERSION%" GEQ "10.0" (
    echo Windows 10 or newer detected. Compatible!
) else (
    echo WARNING: This tool works best with Windows 10 or newer.
    echo You may encounter compatibility issues with older Windows versions.
)

echo.

REM Check if Visual Studio build tools are available
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: Visual Studio C++ compiler not found in PATH.
    echo You will need Visual Studio Build Tools to compile the tool.
    echo Download from: https://visualstudio.microsoft.com/visual-cpp-build-tools/
) else (
    echo Visual Studio C++ compiler found. You can compile the tool.
)

echo.

REM Check if 64-bit building is possible
if defined VSCMD_ARG_TGT_ARCH (
    if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" (
        echo You are in a proper x64 Native Tools Command Prompt. Excellent!
    ) else (
        echo WARNING: You are in a Visual Studio Command Prompt, but not the x64 one.
        echo Please run the "x64 Native Tools Command Prompt for VS 2022" as Administrator.
    )
) else (
    echo WARNING: You are not in a Visual Studio Command Prompt.
    echo Please run the "x64 Native Tools Command Prompt for VS 2022" as Administrator.
)

echo.
echo If you encounter any issues, use troubleshoot.bat for diagnostics.
echo.
echo Compatibility check complete.

pause
