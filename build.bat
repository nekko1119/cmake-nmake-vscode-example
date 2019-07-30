@echo off
cd /d %~dp0

cd build
cmake -G "NMake Makefiles" ..
nmake
set result=%errorlevel%
cd ..
if %result% equ 0 (
  bin\vscodecpp.exe
)
