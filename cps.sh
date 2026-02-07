#!/bin/sh

rm -rf src
mkdir src

cp -p Bible.pro src
cp -p Bible_bg.qm src
cp -p Bible_bg.ts src
cp -p BibleIcon.rc src
cp -p bibleWindow.cpp src
cp -p bibleWindow.h src
cp -p build-update.bat src
cp -p build.bat src
cp -p build.sh src
cp -p concordance.cpp src
cp -p concordance.h src
cp -p copying.txt src
cp -p cpm.sh src
cp -p cpr.bat src
cp -p cps.bat src
cp -p cps.sh src
cp -p exportDialog.cpp src
cp -p exportDialog.h src
cp -p exportDialog.ui src
cp -p fileDownloader.cpp src
cp -p fileDownloader.h src
cp -p filterDialog.cpp src
cp -p filterDialog.h src
cp -p filterDialog.ui src
cp -p history.cpp src
cp -p history.h src
cp -p language.cpp src
cp -p language.h src
cp -p main.cpp src
cp -p mac.sh src
cp -p mainWindow.cpp src
cp -p mainWindow.h src
cp -p mainWindow.ui src
cp -p myFileRoutines.cpp src
cp -p myFileRoutines.h src
cp -p myListView.cpp src
cp -p myListView.h src
cp -p preferedColors.cpp src
cp -p preferedColors.h src
cp -p preferedColors.ui src
cp -p preferences.cpp src
cp -p preferences.h src
cp -p preferences.ui src
cp -p process.cpp src
cp -p process.h src
cp -p README.TXT src
cp -p "Qt-4.6.2-L.bat" src
cp -p showMessage.cpp src
cp -p showMessage.h src
cp -p webUpdater.cpp src
cp -p webUpdater.h src

mkdir src/htdocs
cp -p htdocs/readme-bg.html src/htdocs
cp -p htdocs/readme-en.html src/htdocs

mkdir src/htdocs/images
cp -p htdocs/images/scr-min-3.png src/htdocs/images
cp -p htdocs/images/scr-min-4.png src/htdocs/images

mkdir src/help
cp -p help/help.html src/help
cp -p help/help.odt src/help
cp -p help/help_html_m690fb192.jpg src/help
cp -p help/help_html_m690fb192.png src/help
cp -p help/help_html_m52578ce1.png src/help

mkdir src/data
cp -p data/style.css src/data
cp -p data/style-w.css src/data

mkdir src/data/bibles
cp -p data/bibles/Bible_Structure.txt src/data/bibles
cp -p data/bibles/list-u.txt src/data/bibles
cp -p data/bibles/php-bible.css src/data/bibles

mkdir src/images
cp -p images/Logo_2a.png src/images
cp -p images/Logo_2a.psd src/images
cp -p images/Logo_ikon_3.psd src/images
cp -p images/Logo_ikon_3ico.ico src/images
cp -p images/mac1.icns src/images

mkdir src/msi
cp -p msi/CDBible5.wxs src/msi

mkdir src/update
cp -p update/update.pro src/update
cp -p update/update_bg.qm src/update
cp -p update/update_bg.ts src/update
cp -p update/mainWindow.cpp src/update
cp -p update/mainWindow.h src/update
cp -p update/update.cpp src/update
