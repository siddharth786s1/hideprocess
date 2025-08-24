@echo off
setlocal
echo ===============================================================
echo Building Windows Process Tool (x64)
echo ===============================================================
echo.

REM Ensure proper environment
call "%~dp0verify_64bit.bat" || goto :end_bad

echo Compiling win_injector.c (x64)...
cl /nologo /W4 /Zi /EHsc /DUNICODE /D_UNICODE ^
   /DSTRICT /DWIN32_LEAN_AND_MEAN ^
   /favor:INTEL64 /Qspectre ^
   win_injector.c ^
   /link /MACHINE:X64 /OUT:win_injector.exe

if errorlevel 1 goto :end_bad

echo Compiling win_process_hider.c (x64)...
if exist win_process_hider.c (
  cl /nologo /W4 /Zi /EHsc /DUNICODE /D_UNICODE ^
     /DSTRICT /DWIN32_LEAN_AND_MEAN ^
     /favor:INTEL64 /Qspectre ^
     /LD win_process_hider.c ^
     /link /MACHINE:X64 /OUT:win_process_hider.dll
  if errorlevel 1 goto :end_bad
) else (
  echo [WARN] win_process_hider.c not present; skipping DLL.
)

echo.
call "%~dp0check_binaries.bat"
if errorlevel 1 goto :end_bad

echo.
echo [OK] Build completed successfully.
exit /b 0

:end_bad
echo.
echo [ERROR] Build failed or environment not x64.
exit /b 1