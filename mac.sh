#!/bin/sh

mkdir Bible.app/Contents/Frameworks

FRDIR=/Library/Frameworks/
FRLIN=

cp -R $FRDIR\QtGui.framework Bible.app/Contents/Frameworks
install_name_tool -change $FRLIN\QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui Bible.app/Contents/MacOs/Bible

cp -R $FRDIR\QtCore.framework Bible.app/Contents/Frameworks
install_name_tool -change $FRLIN\QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore Bible.app/Contents/MacOs/Bible

cp -R $FRDIR\QtNetwork.framework Bible.app/Contents/Frameworks
install_name_tool -change $FRLIN\QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork Bible.app/Contents/MacOs/Bible

install_name_tool -change $FRLIN\QtCore.framework/Versions/4/QtCore @executable_path/../../../Bible.app/Contents/Frameworks/QtCore.framework/Versions/4/QtCore Bible.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui
install_name_tool -change $FRLIN\QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore Bible.app/Contents/MacOs/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork

install_name_tool -change $FRLIN\QtGui.framework/Versions/4/QtGui @executable_path/../../../Bible.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui update.app/Contents/MacOs/update
install_name_tool -change $FRLIN\QtCore.framework/Versions/4/QtCore @executable_path/../../../Bible.app/Contents/Frameworks/QtCore.framework/Versions/4/QtCore update.app/Contents/MacOs/update

rm a.zip
zip a.zip Bible_bg.qm data/bibles/list-u.txt data/bibles/Ort/_Diff_.txt data/bibles/Veren/_Diff_.txt data/bibles/38/_Diff_.txt data/bibles/Tzrg/_Diff_.txt data/bibles/Srb/_Diff_.txt data/bibles/Ru/_Diff_.txt data/bibles/Rom/_Diff_.txt data/bibles/KJV/_Diff_.txt data/bibles/KjvSn/_Diff_.txt data/bibles/Asv/_Diff_.txt data/bibles/Vul/_Diff_.txt Bible.app/Contents/MacOS/Bible Bible.app/Contents/Resources/mac1.icns images/Logo_2a.png htdocs/readme-bg.html htdocs/readme-en.html htdocs/images/scr-min-3.png htdocs/images/scr-min-4.png
 
