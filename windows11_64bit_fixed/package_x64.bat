@echo off
REM Package all 64-bit files for transfer to Windows

echo ===============================================================
echo Packaging Windows 11 64-bit Process Hiding Tool
echo ===============================================================
echo.

REM Check if zip command is available
where zip >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Zip command not found. Using PowerShell for compression...
    
    REM Create directory for files
    if not exist windows11_64bit_tool mkdir windows11_64bit_tool
    
    REM Copy files to directory
    echo Copying files to package directory...
    copy win_process_hider_x64.c windows11_64bit_tool\
    copy win_injector_x64.c windows11_64bit_tool\
    copy build_x64.bat windows11_64bit_tool\
    copy run_tool_x64.bat windows11_64bit_tool\
    copy check_64bit.bat windows11_64bit_tool\
    copy troubleshoot.bat windows11_64bit_tool\
    copy WINDOWS11_USAGE.md windows11_64bit_tool\README.md
    
    REM Try PowerShell compression
    echo Compressing using PowerShell...
    powershell -command "Compress-Archive -Path windows11_64bit_tool\* -DestinationPath windows11_64bit_tool.zip -Force"
    
    if %ERRORLEVEL% NEQ 0 (
        echo ERROR: PowerShell compression failed.
        echo Please manually compress the windows11_64bit_tool directory.
    ) else (
        echo Successfully created windows11_64bit_tool.zip
    )
    
    echo Cleaning up temporary directory...
    rmdir /s /q windows11_64bit_tool
) else (
    REM Use zip command
    echo Creating package using zip...
    zip -j windows11_64bit_tool.zip win_process_hider_x64.c win_injector_x64.c build_x64.bat run_tool_x64.bat check_64bit.bat troubleshoot.bat WINDOWS11_USAGE.md
    
    if %ERRORLEVEL% NEQ 0 (
        echo ERROR: Failed to create zip file.
    ) else (
        echo Successfully created windows11_64bit_tool.zip
    )
)

echo.
echo Package created: windows11_64bit_tool.zip
echo Transfer this file to your Windows 11 system and follow instructions in WINDOWS11_USAGE.md

pause
