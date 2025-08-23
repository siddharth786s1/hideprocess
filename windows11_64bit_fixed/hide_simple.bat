@echo off
:: Simple process hiding tool - Simplified version
:: This is a minimal version that focuses just on hiding the process

if "%1"=="" (
  echo Usage: hide_simple.bat ^<process_name^>
  echo Example: hide_simple.bat notepad.exe
  exit /b
)

echo Running process hiding tool for %1...

if not exist win_process_hider.dll (
  echo ERROR: win_process_hider.dll not found. Please build first.
  exit /b
)

if not exist win_injector.exe (
  echo ERROR: win_injector.exe not found. Please build first.
  exit /b
)

win_injector.exe %1
echo Done!
