@echo off
REM This quickly switches to x64 architecture
echo Switching to x64 architecture...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
echo Current architecture: %VSCMD_ARG_TGT_ARCH%
