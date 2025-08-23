@echo off
REM 64-bit verification script for Windows Process Hiding Tool

echo ===============================================================
echo 64-bit Environment Verification Tool
echo ===============================================================
echo.

echo Checking Windows architecture...
if defined ProgramFiles(x86) (
    echo [OK] Running on 64-bit Windows
) else (
    echo [ERROR] Not running on 64-bit Windows
    echo You need a 64-bit Windows system for this tool to work correctly.
    goto end
)

echo.
echo Checking command prompt type...
if defined VSCMD_ARG_TGT_ARCH (
    echo [OK] Running in a Visual Studio Command Prompt
    echo Current architecture target: %VSCMD_ARG_TGT_ARCH%
    
    if /I "%VSCMD_ARG_TGT_ARCH%"=="x64" (
        echo [OK] Correctly using x64 architecture target
    ) else (
        echo [ERROR] Not using x64 architecture target
        echo Please use the x64 Native Tools Command Prompt
        goto end
    )
) else (
    echo [ERROR] Not running in a Visual Studio Command Prompt
    echo Please use the x64 Native Tools Command Prompt
    goto end
)

echo.
echo Checking for 64-bit compiler...
where cl >nul 2>&1
if %errorlevel% equ 0 (
    echo [OK] Found Visual C++ compiler
    
    REM Create a test file
    echo #include ^<stdio.h^> > test_arch.c
    echo int main() { printf("%%d\n", sizeof(void*)); return 0; } >> test_arch.c
    
    REM Compile and run it
    cl /nologo test_arch.c /out:test_arch.exe >nul 2>&1
    for /f %%i in ('test_arch.exe') do set PTR_SIZE=%%i
    
    if "%PTR_SIZE%"=="8" (
        echo [OK] Compiler is generating 64-bit code (pointer size: 8 bytes)
    ) else (
        echo [ERROR] Compiler is NOT generating 64-bit code (pointer size: %PTR_SIZE% bytes)
        echo Please use the x64 Native Tools Command Prompt
        goto cleanup
    )
    
    REM Clean up
    del test_arch.c test_arch.exe test_arch.obj >nul 2>&1
) else (
    echo [ERROR] Visual C++ compiler not found
    echo Please use the x64 Native Tools Command Prompt
    goto end
)

echo.
echo Checking for source files...
if exist win_process_hider_x64.c (
    echo [OK] Found win_process_hider_x64.c
) else (
    echo [ERROR] Could not find win_process_hider_x64.c
    echo This file is required for 64-bit compilation.
    goto end
)

if exist win_injector_x64.c (
    echo [OK] Found win_injector_x64.c
) else (
    echo [ERROR] Could not find win_injector_x64.c
    echo This file is required for 64-bit compilation.
    goto end
)

echo.
echo [SUCCESS] Your environment is correctly set up for 64-bit compilation.
echo You can now run build_x64.bat to compile the 64-bit version.
echo.
echo After building, use run_tool_x64.bat to run the tool.
goto end

:cleanup
del test_arch.c test_arch.exe test_arch.obj >nul 2>&1

:end
echo.
echo ===============================================================
pause
