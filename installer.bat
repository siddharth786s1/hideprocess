@echo off
:: Windows Process Hiding Tool - All-in-One Installer
:: Educational Purpose Only

echo ===============================================================
echo Windows Process Hiding Tool - INSTALLER
echo EDUCATIONAL PURPOSE ONLY
echo ===============================================================
echo.
echo This installer will:
echo  1. Check if the necessary build tools are available
echo  2. Build the process hiding components
echo  3. Set up the tool for easy usage
echo.
echo DISCLAIMER: This tool is for educational and security research 
echo purposes only. Unauthorized use to hide malicious processes is
echo illegal and unethical.
echo.
pause

:: Check if running as administrator
echo Checking administrator privileges...
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: This installer requires administrator privileges.
    echo Please right-click and select "Run as administrator"
    echo.
    pause
    exit /b 1
)

:: Check for Visual Studio build tools
echo Checking for build tools...
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Microsoft Visual C++ compiler (cl.exe) not found.
    echo.
    echo This tool requires Visual Studio Build Tools.
    echo.
    echo Please install Visual Studio with C++ development tools
    echo or the standalone Build Tools from:
    echo https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo.
    echo After installing, run this installer from a
    echo "Developer Command Prompt for VS"
    echo.
    pause
    exit /b 1
)

:: Build the components
echo.
echo Building components...
echo.

call build.bat

:: Check if build was successful
if not exist win_process_hider.dll (
    echo.
    echo ERROR: Build failed. Please check for errors above.
    echo.
    pause
    exit /b 1
)

echo.
echo Installation completed successfully!
echo.
echo To hide a process:
echo   hide_process.bat ^<process_name_to_hide^>
echo.
echo Example:
echo   hide_process.bat notepad.exe
echo.
echo IMPORTANT: Always run the tool as Administrator
echo.
pause
