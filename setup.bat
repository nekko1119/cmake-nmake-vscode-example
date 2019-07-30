@echo off
cd /d %~dp0

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

if exist "build" (
  rmdir /S /Q "build"
)

mkdir build

call build.bat
