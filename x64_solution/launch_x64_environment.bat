@echo off
REM Batch file to create and open the correct x64 command prompt

echo ===============================================================
echo Windows Process Hiding Tool - Launch x64 Environment
echo ===============================================================
echo.
echo This script will:
echo 1. Check if Visual Studio is installed
echo 2. Create a temporary batch file to launch the x64 command prompt
echo 3. Open it with Administrator privileges
echo.
echo Press any key to continue...
pause >nul

REM Check for Visual Studio installations
set FOUND_VS=0

REM Check for VS 2022
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" (
    echo Found Visual Studio 2022 Professional
    set VS_PATH="%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"
    set FOUND_VS=1
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" (
    echo Found Visual Studio 2022 Enterprise
    set VS_PATH="%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat"
    set FOUND_VS=1
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
    echo Found Visual Studio 2022 Community
    set VS_PATH="%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
    set FOUND_VS=1
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" (
    echo Found Visual Studio 2022 Build Tools
    set VS_PATH="%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
    set FOUND_VS=1
)

REM Check for VS 2019 if 2022 not found
if %FOUND_VS%==0 (
    if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat" (
        echo Found Visual Studio 2019 Professional
        set VS_PATH="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat"
        set FOUND_VS=1
    ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat" (
        echo Found Visual Studio 2019 Enterprise
        set VS_PATH="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat"
        set FOUND_VS=1
    ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" (
        echo Found Visual Studio 2019 Community
        set VS_PATH="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
        set FOUND_VS=1
    ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat" (
        echo Found Visual Studio 2019 Build Tools
        set VS_PATH="%ProgramFiles(x86)%\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"
        set FOUND_VS=1
    )
)

REM If no VS installation was found
if %FOUND_VS%==0 (
    echo ERROR: Could not find Visual Studio installation.
    echo Please install Visual Studio or Visual Studio Build Tools with C++ development components.
    echo.
    echo Download from: https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022
    pause
    exit /b 1
)

REM Create a temporary batch file to launch x64 environment
echo Creating temporary launch script...
echo @echo off > "%TEMP%\launch_x64_env.bat"
echo call %VS_PATH% -arch=amd64 >> "%TEMP%\launch_x64_env.bat"
echo cd /d "%CD%" >> "%TEMP%\launch_x64_env.bat"
echo echo. >> "%TEMP%\launch_x64_env.bat"
echo echo ================================================================= >> "%TEMP%\launch_x64_env.bat"
echo echo x64 Development Environment Successfully Launched! >> "%TEMP%\launch_x64_env.bat"
echo echo Current Architecture: x64 >> "%TEMP%\launch_x64_env.bat"
echo echo Current Directory: %%CD%% >> "%TEMP%\launch_x64_env.bat"
echo echo. >> "%TEMP%\launch_x64_env.bat"
echo echo You can now run: >> "%TEMP%\launch_x64_env.bat"
echo echo   verify_64bit.bat >> "%TEMP%\launch_x64_env.bat"
echo echo   build_x64.bat >> "%TEMP%\launch_x64_env.bat"
echo echo   run_tool_x64.bat notepad.exe >> "%TEMP%\launch_x64_env.bat"
echo echo ================================================================= >> "%TEMP%\launch_x64_env.bat"
echo echo. >> "%TEMP%\launch_x64_env.bat"

REM Launch the batch file with admin privileges
echo Launching x64 command prompt with administrator privileges...
echo (A UAC prompt may appear requesting permission)
powershell -Command "Start-Process cmd.exe -ArgumentList '/k %TEMP%\launch_x64_env.bat' -Verb RunAs"

echo.
echo If the x64 command prompt opened successfully:
echo 1. Run verify_64bit.bat to verify your environment
echo 2. Run build_x64.bat to build the tool
echo 3. Run run_tool_x64.bat notepad.exe to test

pause
