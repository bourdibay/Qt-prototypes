@echo off

set directoryFinal="build_vs11_32bits"
if exist %directoryFinal% rmdir /Q /S %directoryFinal%

rem Launch the creation of the VS solution first
mkdir %directoryFinal%
cd %directoryFinal%

cmake -G "Visual Studio 11" ../

pause
