@echo off
REM Package script for Windows 11 64-bit Process Hiding Tool

echo ===============================================================
echo Creating Windows 11 64-bit Process Hiding Tool Package
echo ===============================================================
echo.

REM Create a temporary directory
if not exist win11_64bit_package mkdir win11_64bit_package

REM Copy all necessary files
echo Copying files to package directory...
copy win_process_hider_x64.c win11_64bit_package\
copy win_injector_x64.c win11_64bit_package\
copy build_x64.bat win11_64bit_package\
copy run_tool_x64.bat win11_64bit_package\
copy verify_64bit.bat win11_64bit_package\
copy check_64bit.bat win11_64bit_package\
copy WINDOWS11_64BIT_GUIDE.md win11_64bit_package\README.md

REM Create a simple installer batch file
echo Creating installer script...
echo @echo off > win11_64bit_package\INSTALL.bat
echo echo ================================================================= >> win11_64bit_package\INSTALL.bat
echo echo Windows 11 64-bit Process Hiding Tool - Installation >> win11_64bit_package\INSTALL.bat
echo echo ================================================================= >> win11_64bit_package\INSTALL.bat
echo echo. >> win11_64bit_package\INSTALL.bat
echo echo This script will verify your environment and build the tool. >> win11_64bit_package\INSTALL.bat
echo echo. >> win11_64bit_package\INSTALL.bat
echo echo IMPORTANT: You must run this from an x64 Native Tools Command Prompt! >> win11_64bit_package\INSTALL.bat
echo echo. >> win11_64bit_package\INSTALL.bat
echo echo Press any key to continue... >> win11_64bit_package\INSTALL.bat
echo pause >> win11_64bit_package\INSTALL.bat
echo echo. >> win11_64bit_package\INSTALL.bat
echo echo Step 1: Verifying environment... >> win11_64bit_package\INSTALL.bat
echo call verify_64bit.bat >> win11_64bit_package\INSTALL.bat
echo echo. >> win11_64bit_package\INSTALL.bat
echo echo Step 2: Building tool... >> win11_64bit_package\INSTALL.bat
echo call build_x64.bat >> win11_64bit_package\INSTALL.bat
echo echo. >> win11_64bit_package\INSTALL.bat
echo echo Step 3: Installation complete! >> win11_64bit_package\INSTALL.bat
echo echo. >> win11_64bit_package\INSTALL.bat
echo echo To use the tool, run: >> win11_64bit_package\INSTALL.bat
echo echo   run_tool_x64.bat ^<process_name^> >> win11_64bit_package\INSTALL.bat
echo echo   Example: run_tool_x64.bat notepad.exe >> win11_64bit_package\INSTALL.bat
echo echo. >> win11_64bit_package\INSTALL.bat
echo echo Press any key to exit... >> win11_64bit_package\INSTALL.bat
echo pause >> win11_64bit_package\INSTALL.bat

REM Create ZIP file
echo Creating ZIP package...
zip -r windows11_64bit_fixed.zip win11_64bit_package/*

REM Clean up
echo Cleaning up temporary directory...
rmdir /s /q win11_64bit_package

echo.
echo Package created: windows11_64bit_fixed.zip
echo.
echo INSTRUCTIONS:
echo 1. Transfer this ZIP file to your Windows 11 system
echo 2. Extract all files to a folder
echo 3. Open "x64 Native Tools Command Prompt for VS 2022" as Administrator
echo 4. Navigate to the extracted folder
echo 5. Run INSTALL.bat
echo 6. Follow the on-screen instructions
echo.
echo This package contains all the necessary files to fix the 16-bit application error.

"$BROWSER" "file://$(pwd)"
