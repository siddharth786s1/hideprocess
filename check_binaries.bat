@echo off
setlocal
echo ===============================================================
echo Verifying PE headers (expect: Machine = x64)
echo ===============================================================

if not exist win_injector.exe (
  echo [ERROR] win_injector.exe not found.
  exit /b 1
)

where dumpbin >nul 2>&1
if errorlevel 1 (
  echo [WARN] dumpbin not available; cannot confirm headers.
  exit /b 0
)

for /f "tokens=1,* delims=:" %%A in ('dumpbin /headers win_injector.exe ^| findstr /i /c:"machine"') do (
  echo %%A:%%B
)

dumpbin /headers win_injector.exe | findstr /i "x64" >nul
if errorlevel 1 (
  echo [ERROR] win_injector.exe is NOT x64. Reopen the x64 Native Tools prompt and rebuild.
  exit /b 1
) else (
  echo [OK] win_injector.exe is x64
)

if exist win_process_hider.dll (
  for /f "tokens=1,* delims=:" %%A in ('dumpbin /headers win_process_hider.dll ^| findstr /i /c:"machine"') do (
    echo %%A:%%B
  )
  dumpbin /headers win_process_hider.dll | findstr /i "x64" >nul
  if errorlevel 1 (
    echo [ERROR] win_process_hider.dll is NOT x64.
    exit /b 1
  ) else (
    echo [OK] win_process_hider.dll is x64
  )
)
exit /b 0
