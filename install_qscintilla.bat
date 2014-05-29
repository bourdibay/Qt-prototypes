@echo off
rem copyright (c) 2013 Benjamin Bourdin benjamin.bourdin40@gmail.com
rem
rem Redistribution and use is allowed according to the terms of the FreeBSD license.

if not exist "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" goto novcvarsall
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x64

if exist "scintilla.zip" del "scintilla.zip"
if exist "QScintilla-gpl-2.8" rmdir /Q /S "QScintilla-gpl-2.8"

echo Download of QScintilla-2.8 and unzip it.

@powershell -NoProfile -ExecutionPolicy unrestricted -Command "& {" ^
 " function unzip($filename) { " ^
 " if (!(test-path $filename)) { throw \"$filename does not exist\" } " ^
 " $shell = new-object -com shell.application ; " ^
 " $shell.namespace($pwd.path).copyhere($shell.namespace((join-path $pwd $filename)).items()) " ^
 " } " ^
 " $source = \"http://sourceforge.net/projects/pyqt/files/QScintilla2/QScintilla-2.8/QScintilla-gpl-2.8.zip\" ; " ^
 " $destination = \"scintilla.zip\" ; " ^
 " $wc = New-Object System.Net.WebClient ; " ^
 " $wc.DownloadFile($source, $destination) ; " ^
 " unzip($destination) ; " ^
 "}"
echo Done

set dir_scintilla="QScintilla-gpl-2.8\Qt4Qt5\"
set curr_dir=%~dp0

rem You have to modify the Makefile.Debug in order to have the lines as followed :
rem QMAKE_TARGET  = qscintilla2_d
rem DESTDIR        = debug\ #avoid trailing-slash linebreak
rem TARGET         = qscintilla2_d.dll
rem DESTDIR_TARGET = debug\qscintilla2_d.dll

cd %dir_scintilla%
echo Launch qmake
qmake qscintilla.pro
echo Done

echo Modification of the Makefile.Debug rules

@powershell -NoProfile -ExecutionPolicy unrestricted -Command "& {" ^
 " $file = \"Makefile.Debug\" ; " ^
 " $new_file = \"Makefile.Debug.modified\" ; " ^
 " (Get-Content $file) | Foreach-Object { " ^
 "   $_ -replace 'QMAKE_TARGET  = qscintilla2', 'QMAKE_TARGET  = qscintilla2_d' ` " ^
 "      -replace 'TARGET         = qscintilla2.dll', 'TARGET         = qscintilla2_d.dll' ` " ^
 "      -replace 'DESTDIR_TARGET = debug\\qscintilla2.dll', 'DESTDIR_TARGET = debug\\qscintilla2_d.dll' ` " ^
 "   } | Set-Content $new_file " ^
 "}"

if exist "Makefile.Debug" del "Makefile.Debug"
if exist "Makefile.Debug.modified" ren "Makefile.Debug.modified" "Makefile.Debug"
 
echo Done

echo Launch nmake (debug, release) 
nmake debug
nmake release
nmake release-install
echo Done

echo Install files to QTDIR (%QTDIR%)
xcopy /s /q /y /i "debug\qscintilla2_d.lib" "%QTDIR%\lib\qscintilla2_d.lib"
xcopy /s /q /y /i "debug\qscintilla2_d.dll" "%QTDIR%\lib\qscintilla2_d.dll"
xcopy /s /q /y /i "debug\qscintilla2_d.pdb" "%QTDIR%\lib\qscintilla2_d.pdb"
xcopy /s /q /y /i "debug\qscintilla2_d.dll" "%QTDIR%\bin\qscintilla2_d.dll"

xcopy /s /q /y /i "release\qscintilla2.dll" "%QTDIR%\bin\qscintilla2.dll"
echo Done

cd %curr_dir%

rem if exist "scintilla.zip" del "scintilla.zip"
rem if exist "QScintilla-gpl-2.8" rmdir /Q /S "QScintilla-gpl-2.8"

pause
goto :eof

:novcvarsall
echo "Fail to find vcvarsall.bat"

pause
goto :eof