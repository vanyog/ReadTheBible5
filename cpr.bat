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
copy build\release\Bible.exe %OUTDIR%
copy *.qm %OUTDIR%
copy 3rdparty\zlib\build\Release\z.dll %OUTDIR%

mkdir %OUTDIR%\htdocs
robocopy htdocs %OUTDIR%\htdocs /MIR

mkdir %OUTDIR%\images
copy images\Logo_2a.png %OUTDIR%\images

mkdir %OUTDIR%\data
copy data\style.css %OUTDIR%\data
copy data\style-w.css %OUTDIR%\data

mkdir %OUTDIR%\data\bibles
copy data\bibles\list-u.txt %OUTDIR%\data\bibles
copy data\bibles\Bible_Structure.txt %OUTDIR%\data\bibles
copy data\bibles\php-bible.css %OUTDIR%\data\bibles
copy data\bibles\style.css %OUTDIR%\data\bibles

cd %OUTDIR%
%QTDIR%\bin\windeployqt Bible.exe

cd ..\..
if exist "%ZIPFILE%" del "%ZIPFILE%"
powershell Compress-Archive -Path %OUTDIR%\* -DestinationPath %ZIPFILE%

endlocal
echo All done
pause
