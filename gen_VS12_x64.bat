@echo off

set directoryFinal="build_vs12_x64"
if exist %directoryFinal% rmdir /Q /S %directoryFinal%

rem Launch the creation of the VS solution first
mkdir %directoryFinal%
cd %directoryFinal%

cmake -G "Visual Studio 12 Win64" ../

pause
