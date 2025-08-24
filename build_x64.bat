@echo off
setlocal enabledelayedexpansion

echo ===============================================================
echo Windows Process Hiding Tool - BULLETPROOF BUILD SCRIPT
echo ===============================================================

REM Check if we're in x64 environment
if not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo [ERROR] Not in x64 build environment!
    echo Please run from "x64 Native Tools Command Prompt for VS 2022"
    pause
    exit /b 1
)

REM Clean previous builds
if exist win_injector.exe del win_injector.exe
if exist win_process_hider.dll del win_process_hider.dll
if exist *.obj del *.obj
if exist *.pdb del *.pdb

REM Check if source files exist
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

echo [INFO] Building win_process_hider.dll first...
cl /nologo /W3 /O2 /MT /DUNICODE /D_UNICODE ^
   /LD win_process_hider.c ^
   /link /MACHINE:X64 /OUT:win_process_hider.dll ^
   /DEF:NUL ^
   kernel32.lib user32.lib

if !errorlevel! neq 0 (
    echo [ERROR] Failed to build win_process_hider.dll
    echo [INFO] Check compiler output above for details
    pause
    exit /b 1
)

echo [INFO] Building win_injector.exe...
cl /nologo /W3 /O2 /MT /DUNICODE /D_UNICODE ^
   win_injector.c ^
   /link /MACHINE:X64 /OUT:win_injector.exe ^
   kernel32.lib user32.lib psapi.lib

if !errorlevel! neq 0 (
    echo [ERROR] Failed to build win_injector.exe
    echo [INFO] Check compiler output above for details
    pause
    exit /b 1
)

echo [SUCCESS] Build completed successfully!
echo [INFO] Files created:
if exist win_injector.exe echo   ✓ win_injector.exe
if exist win_process_hider.dll echo   ✓ win_process_hider.dll

REM Verify they're actually 64-bit
echo [INFO] Verifying 64-bit binaries...
where dumpbin >nul 2>&1
if !errorlevel! equ 0 (
    echo [INFO] Running dumpbin verification...
    dumpbin /headers win_injector.exe | findstr "machine" | findstr "x64" >nul
    if !errorlevel! equ 0 (
        echo   ✓ win_injector.exe is 64-bit
    ) else (
        echo   ❌ win_injector.exe is NOT 64-bit
        pause
        exit /b 1
    )
    
    dumpbin /headers win_process_hider.dll | findstr "machine" | findstr "x64" >nul
    if !errorlevel! equ 0 (
        echo   ✓ win_process_hider.dll is 64-bit
    ) else (
        echo   ❌ win_process_hider.dll is NOT 64-bit
        pause
        exit /b 1
    )
) else (
    echo [INFO] dumpbin not available, skipping binary verification
)

echo.
echo [SUCCESS] All builds completed successfully!
echo [INFO] You can now run: win_injector.exe notepad.exe
pause