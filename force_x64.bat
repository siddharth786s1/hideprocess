@echo off
setlocal
echo ===============================================================
echo Forcing VS toolchain to x64 target
echo ===============================================================
echo.

if not defined VSINSTALLDIR (
  echo [ERROR] Not a Visual Studio command prompt.
  echo Open "x64 Native Tools Command Prompt for VS 2022" as Administrator.
  exit /b 1
)

REM Try the installed vcvarsall.bat to switch to x64
if exist "%VSINSTALLDIR%VC\Auxiliary\Build\vcvarsall.bat" (
  call "%VSINSTALLDIR%VC\Auxiliary\Build\vcvarsall.bat" x64
) else (
  echo [ERROR] Could not locate vcvarsall.bat under %VSINSTALLDIR%.
  echo Please repair VS Build Tools.
  exit /b 1
)

if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" (
  echo [OK] Switched to x64 toolset.
  exit /b 0
) else (
  echo [ERROR] Failed to switch to x64.
  exit /b 1
)
