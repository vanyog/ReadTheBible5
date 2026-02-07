path d:\windows\system32

echo off
echo This script creates a new directory rel and copies files in it which are necessary for  Bible.exe to execute. Close it you if don't like to do that or
pause

echo on
rmdir /s/q rel

mkdir rel
copy C:\Users\vanyog\Documents\build-Bible-Desktop_Qt_4_8_5-Release\release\Bible.exe rel
copy Bible_bg.qm rel
copy Bible_mk.qm rel
copy copying.txt rel
copy unzip.exe rel
copy C:\Qt\4.8.5\bin\mingwm10.dll rel
copy C:\Qt\4.8.5\bin\QtCore4.dll rel
copy C:\Qt\4.8.5\bin\QtGui4.dll rel
copy C:\Qt\4.8.5\bin\QtNetwork4.dll rel
copy C:\Qt\4.8.5\bin\libgcc_s_dw2-1.dll rel


mkdir rel\htdocs
copy htdocs\readme-bg.html rel\htdocs
copy htdocs\readme-en.html rel\htdocs
copy htdocs\style.css rel\htdocs
copy htdocs\copying.txt rel\htdocs

mkdir rel\htdocs\images
copy htdocs\images\scr-min-3.png rel\htdocs\images
copy htdocs\images\scr-min-4.png rel\htdocs\images

mkdir rel\htdocs\help
copy htdocs\help\*.* rel\htdocs\help
del rel\htdocs\help\help.odt

mkdir rel\images
copy images\Logo_2a.png rel\images

mkdir rel\data
copy data\style-w.css rel\data

mkdir rel\data\bibles
copy data\bibles\list-u.txt rel\data\bibles
copy data\bibles\Bible_Structure.txt rel\data\bibles

pause
