@echo off
setlocal enabledelayedexpansion
echo ===============================================================
echo 64-bit Environment Verification Tool
echo ===============================================================
echo.

REM 1) OS
echo [1/4] Checking Windows architecture...
if defined ProgramFiles(x86) (
  echo   [OK] 64-bit Windows detected
) else (
  echo   [ERROR] This system is not 64-bit Windows
  goto :end_bad
)

REM 2) VS prompt + target arch
echo.
echo [2/4] Checking Visual Studio command prompt...
if defined VSINSTALLDIR (
  echo   [OK] VS command prompt detected
) else (
  echo   [ERROR] Not a VS command prompt. Open "x64 Native Tools Command Prompt for VS 2022" as Administrator.
  goto :end_bad
)

if defined VSCMD_ARG_TGT_ARCH (
  echo   Current target architecture: %VSCMD_ARG_TGT_ARCH%
  if /I not "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    echo   [ERROR] Using x86 target. Run: force_x64.bat
    goto :end_bad
  ) else (
    echo   [OK] x64 target in effect
  )
) else (
  echo   [WARN] VS env present but no VSCMD_ARG_TGT_ARCH. Run: force_x64.bat
  goto :end_bad
)

REM 3) Compiler
echo.
echo [3/4] Checking for MSVC compiler (cl)...
where cl >nul 2>&1
if errorlevel 1 (
  echo   [ERROR] cl.exe not found. Install VS Build Tools or reopen the VS x64 Native Tools prompt.
  goto :end_bad
) else (
  echo   [OK] cl.exe found
)

REM 4) dumpbin (optional but useful)
echo.
echo [4/4] Checking dumpbin (for binary header checks)...
where dumpbin >nul 2>&1
if errorlevel 1 (
  echo   [WARN] dumpbin not found. Header checks will be limited.
) else (
  echo   [OK] dumpbin found
)

echo.
echo All checks passed for 64-bit build.
goto :eof

:end_bad
echo.
echo One or more checks failed. Open "x64 Native Tools Command Prompt for VS 2022"
echo as Administrator, then run: force_x64.bat, verify_64bit.bat, build_x64.bat
exit /b 1
