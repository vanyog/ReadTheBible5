@echo off
setlocal

echo Make deployment for Read the Bible App
pause

set OUTDIR=build\rel
set QTDIR=C:\Qt\6.10.2\msvc2022_64
if "%PROCESSOR_ARCHITECTURE%"=="ARM64" (
    set QTDIR=C:\Qt\6.10.2\msvc2022_arm64
)
set ZIPFILE=build\Bible5.zip

rmdir /s/q %OUTDIR%

mkdir %OUTDIR%
copy build\release\Bible5.exe %OUTDIR%
copy 3rdparty\zlib\build\Release\z.dll %OUTDIR%

cd %OUTDIR%
%QTDIR%\bin\windeployqt Bible5.exe

endlocal
echo All done
pause
