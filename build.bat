echo off
echo This script will build the Bible 5 project. Close it you if you don't like to do that or
pause
echo on


qmake
mingw32-make %*
cd update
qmake
mingw32-make %*
copy update.exe ..
cd ..

echo off
echo ========
echo All done.
echo ========
pause