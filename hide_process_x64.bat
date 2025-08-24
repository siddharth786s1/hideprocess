@echo off
echo =============================================
echo Windows Process Hiding Tool - 64-bit Version
echo EDUCATIONAL PURPOSE ONLY
echo =============================================
echo.

if "%1"=="" (
  echo Usage: %0 process_to_hide.exe
  echo Example: %0 notepad.exe
  echo.
  exit /b 1
)

set TARGET_PROCESS=%1
echo Target process to hide: %TARGET_PROCESS%
echo.

echo Checking if Task Manager is running...
tasklist | findstr /i "taskmgr.exe" >nul
if %errorlevel% neq 0 (
  echo Task Manager is not running.
  echo Please start Task Manager before running this tool.
  echo.
  echo Press any key to open Task Manager...
  pause >nul
  start taskmgr.exe
  echo Waiting for Task Manager to start...
  timeout /t 3 /nobreak >nul
)

echo Setting environment variable...
set TARGET_PROCESS=%1
echo.

echo Running process hiding tool...
win_injector.exe %1
echo.

echo Done! If %TARGET_PROCESS% is running, it should now be hidden from Task Manager.
echo.
