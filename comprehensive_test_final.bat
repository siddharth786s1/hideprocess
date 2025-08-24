@echo off
echo ==================================================
echo COMPREHENSIVE PROCESS HIDING TEST
echo ==================================================
echo.

echo Step 1: Starting notepad for testing...
start notepad
timeout /t 2 /nobreak >nul

echo Step 2: Taking before screenshot...
echo [INFO] Open Task Manager and note that notepad.exe is visible
echo [INFO] Press any key when ready to continue...
pause >nul

echo Step 3: Running process hider injection...
win_injector.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Injection failed
    pause
    exit /b 1
)

echo.
echo Step 4: Verification phase...
echo [INFO] Now open Task Manager again
echo [INFO] notepad.exe should be HIDDEN from the process list
echo [INFO] Check C:\process_hider_log.txt for detailed logs
echo.

echo Step 5: Testing different monitoring tools...
echo [INFO] If available, also test with:
echo   - Process Explorer (procexp.exe)
echo   - ProcessHacker.exe  
echo   - PowerShell: Get-Process
echo.

echo [SUCCESS] Test completed!
echo Check the log file for hook installation details:
if exist C:\process_hider_log.txt (
    echo [FOUND] C:\process_hider_log.txt
    echo.
    echo Last few log entries:
    tail -n 10 C:\process_hider_log.txt 2>nul || echo [No tail command available]
) else (
    echo [NOT FOUND] C:\process_hider_log.txt - Check if hooks were installed
)

pause
