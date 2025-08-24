@echo off
echo =========================================
echo CLEAN BUILD SCRIPT - NO ERRORS VERSION
echo =========================================

echo Terminating any processes that might lock files...
taskkill /f /im taskmgr.exe >nul 2>&1
taskkill /f /im procexp.exe >nul 2>&1
taskkill /f /im ProcessHacker.exe >nul 2>&1
taskkill /f /im win_injector.exe >nul 2>&1

echo Waiting for processes to terminate...
timeout /t 3 /nobreak >nul

echo Cleaning old files...
del /f /q *.dll *.exe *.obj *.lib *.exp *.pdb >nul 2>&1

echo.
echo Building DLL...
cl /LD /DUNICODE /D_UNICODE win_process_hider.c ^
   /Fe:win_process_hider.dll ^
   /link /MACHINE:X64 /SUBSYSTEM:WINDOWS ^
   kernel32.lib user32.lib

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: DLL build failed!
    pause
    exit /b 1
)

echo.
echo Building EXE...
cl /DUNICODE /D_UNICODE win_injector.c ^
   /Fe:win_injector.exe ^
   /link /MACHINE:X64 /SUBSYSTEM:CONSOLE ^
   kernel32.lib user32.lib psapi.lib

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: EXE build failed!
    pause
    exit /b 1
)

echo.
echo SUCCESS: Both files built successfully!
echo.
dir *.dll *.exe
echo.
echo Ready to test! Run: win_injector.exe
pause
