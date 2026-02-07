set QTDIR=C:\qt\4.4.3
set QMAKESPEC=C:\qt\4.4.3\mkspecs\win32-g++
set path=C:\qt\4.4.3\bin;C:\MinGW\bin;C:\windows\system32

cd update
qmake
make