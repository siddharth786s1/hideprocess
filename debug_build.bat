@echo off
setlocal enabledelayedexpansion

echo ==================================================
echo DEBUG BUILD SCRIPT FOR WINDOWS PROCESS HIDER
echo ==================================================

echo Checking current directory...
echo Working in: %CD%

echo.
echo Listing relevant files...
if exist win_injector.c (echo [FOUND] win_injector.c) else (echo [MISSING] win_injector.c)
if exist win_process_hider.c (echo [FOUND] win_process_hider.c) else (echo [MISSING] win_process_hider.c)

echo.
echo Checking for Visual Studio environment...
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Visual Studio compiler found
    cl 2>&1 | findstr /i "x64"
    if !ERRORLEVEL! EQU 0 (
        echo [OK] x64 compiler environment detected
    ) else (
        echo [WARNING] x64 environment may not be active
    )
) else (
    echo [ERROR] Visual Studio compiler not found
    echo Please run from "x64 Native Tools Command Prompt"
    pause
    exit /b 1
)

echo.
echo Force-killing processes that might lock files...
taskkill /f /im taskmgr.exe >nul 2>&1
taskkill /f /im procexp.exe >nul 2>&1
taskkill /f /im ProcessHacker.exe >nul 2>&1
taskkill /f /im win_injector.exe >nul 2>&1

echo Waiting 2 seconds for processes to terminate...
timeout /t 2 /nobreak >nul

echo.
echo Removing read-only attributes and deleting old files...
if exist win_process_hider.dll (
    attrib -r win_process_hider.dll
    del /f /q win_process_hider.dll
    echo Deleted old DLL
)
if exist win_injector.exe (
    attrib -r win_injector.exe
    del /f /q win_injector.exe
    echo Deleted old EXE
)

echo.
echo Cleaning up any remaining files...
del /f /q *.obj *.lib *.exp *.pdb >nul 2>&1

echo.
echo ==================================================
echo BUILDING DLL: win_process_hider.dll
echo ==================================================

cl /LD /DUNICODE /D_UNICODE win_process_hider.c /Fe:win_process_hider.dll ^
   /link /MACHINE:X64 /SUBSYSTEM:WINDOWS /NOIMPLIB kernel32.lib user32.lib

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to build DLL
    echo Error code: %ERRORLEVEL%
    echo.
    echo Checking if files are still locked...
    handle win_process_hider.dll 2>nul
    pause
    exit /b 1
) else (
    echo [SUCCESS] DLL built successfully
)

if not exist win_process_hider.dll (
    echo [ERROR] DLL file not created despite successful build
    pause
    exit /b 1
)

echo.
echo ==================================================
echo BUILDING EXE: win_injector.exe
echo ==================================================

cl /DUNICODE /D_UNICODE win_injector.c /Fe:win_injector.exe ^
   /link /MACHINE:X64 /SUBSYSTEM:CONSOLE kernel32.lib user32.lib psapi.lib

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to build EXE
    echo Error code: %ERRORLEVEL%
    pause
    exit /b 1
) else (
    echo [SUCCESS] EXE built successfully
)

echo.
echo ==================================================
echo BUILD COMPLETED SUCCESSFULLY
echo ==================================================

echo Verifying created files...
if exist win_process_hider.dll (
    echo [OK] win_process_hider.dll created (%~zB bytes)
    dir win_process_hider.dll | findstr /v "Directory"
) else (
    echo [ERROR] win_process_hider.dll missing
)

if exist win_injector.exe (
    echo [OK] win_injector.exe created (%~zC bytes) 
    dir win_injector.exe | findstr /v "Directory"
) else (
    echo [ERROR] win_injector.exe missing
)

echo.
echo READY TO TEST!
echo Run: win_injector.exe
echo Then open Task Manager to verify notepad is hidden
echo Check C:\process_hider_log.txt for detailed logs

pause
