@echo off
setlocal enabledelayedexpansion

echo ===============================================================
echo Windows Process Hiding Tool - ADVANCED BUILD SCRIPT
echo ===============================================================

REM Kill any processes that might be using our DLL
echo [INFO] Stopping processes that might lock our files...
taskkill /F /IM taskmgr.exe >nul 2>&1
taskkill /F /IM win_injector.exe >nul 2>&1

REM Wait a moment for processes to fully terminate
timeout /t 2 /nobreak >nul

REM Force delete old files
echo [INFO] Cleaning previous builds...
if exist win_injector.exe (
    attrib -r win_injector.exe
    del /f /q win_injector.exe
)
if exist win_process_hider.dll (
    attrib -r win_process_hider.dll  
    del /f /q win_process_hider.dll
)
if exist *.obj del /f /q *.obj
if exist *.lib del /f /q *.lib
if exist *.exp del /f /q *.exp
if exist *.pdb del /f /q *.pdb

REM Check x64 environment
if not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo [ERROR] Not in x64 build environment!
    pause
    exit /b 1
)

REM Verify source files exist
if not exist "win_injector.c" (
    echo [ERROR] win_injector.c not found!
    pause
    exit /b 1
)
if not exist "win_process_hider.c" (
    echo [ERROR] win_process_hider.c not found!
    pause
    exit /b 1
)

echo [INFO] Building with advanced process hiding...

REM Build the DLL with specific flags to avoid locking issues
echo [INFO] Compiling win_process_hider.dll...
cl /nologo /W3 /O2 /MT /DUNICODE /D_UNICODE ^
   /LD win_process_hider.c ^
   /Fe:win_process_hider.dll ^
   /link /MACHINE:X64 /NOIMPLIB ^
   kernel32.lib user32.lib

if !errorlevel! neq 0 (
    echo [ERROR] Failed to build DLL
    pause
    exit /b 1
)

echo [INFO] Compiling win_injector.exe...
cl /nologo /W3 /O2 /MT /DUNICODE /D_UNICODE ^
   win_injector.c ^
   /Fe:win_injector.exe ^
   /link /MACHINE:X64 ^
   kernel32.lib user32.lib psapi.lib

if !errorlevel! neq 0 (
    echo [ERROR] Failed to build injector
    pause
    exit /b 1
)

echo [SUCCESS] Build completed!
if exist win_injector.exe echo   ✓ win_injector.exe
if exist win_process_hider.dll echo   ✓ win_process_hider.dll

pause
