@echo off
setlocal

echo Make deployment for Read the Bible App
pause

set OUTDIR=build\rel
set QTDIR=C:\Qt\6.11.0\msvc2022_64
if "%PROCESSOR_ARCHITECTURE%"=="ARM64" (
    set QTDIR=C:\Qt\6.10.2\msvc2022_arm64
)

rmdir /s/q %OUTDIR%

mkdir %OUTDIR%
copy build\Release\Bible5.exe %OUTDIR%
copy 3rdparty\zlib\build\Release\z.dll %OUTDIR%

cd %OUTDIR%
%QTDIR%\bin\windeployqt Bible5.exe

endlocal
echo All done
pause
