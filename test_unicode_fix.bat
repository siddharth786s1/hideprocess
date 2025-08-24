@echo off
echo Testing the Unicode fix for process detection
echo ============================================
echo.

echo Building the updated version...
call build_x64.bat
if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo.
echo Testing process detection (without injection for safety)...
echo This should detect Task Manager if it's running:
echo.

win_injector.exe test_process_that_does_not_exist.exe

echo.
echo If you see "Found monitoring tool: taskmgr.exe" above, the fix worked!
echo If not, Task Manager might not be running.
