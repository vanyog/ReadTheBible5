path d:\windows\system32
rmdir /s/q src
pause
mkdir src

copy Bible.pro src
copy Bible_bg.qm src
copy Bible_bg.ts src
copy BibleIcon.rc src
copy bibleWindow.cpp src
copy bibleWindow.h src
copy build.bat src
copy concordance.cpp src
copy concordance.h src
copy copying.txt src
copy cpm.sh src
copy cpr.bat src
copy cps.bat src
copy cps.sh src
copy exportDialog.cpp src
copy exportDialog.h src
copy exportDialog.ui src
copy fileDownloader.cpp src
copy fileDownloader.h src
copy filterDialog.cpp src
copy filterDialog.h src
copy filterDialog.ui src
copy history.cpp src
copy history.h src
copy language.cpp src
copy language.h src
copy main.cpp src
copy mac.sh src
copy mainWindow.cpp src
copy mainWindow.h src
copy mainWindow.ui src
copy myComboBox.cpp src
copy myComboBox.h src
copy myFileRoutines.cpp src
copy myFileRoutines.h src
copy myListView.cpp src
copy myListView.h src
copy preferedColors.cpp src
copy preferedColors.h src
copy preferedColors.ui src
copy preferences.cpp src
copy preferences.h src
copy preferences.ui src
copy process.cpp src
copy process.h src
copy README.TXT src
copy "Qt-4.6.3-L.bat" src
copy showMessage.cpp src
copy showMessage.h src
copy Start-L.bat src
copy unzip.exe src
copy webUpdater.cpp src
copy webUpdater.h src

mkdir src\htdocs
copy htdocs\readme-bg.html src\htdocs
copy htdocs\readme-en.html src\htdocs

mkdir src\htdocs\images
copy htdocs\images\scr-min-3.png src\htdocs\images
copy htdocs\images\scr-min-4.png src\htdocs\images

mkdir src\help
copy help\help.html src\help
copy help\help.odt src\help
copy help\help_html_m690fb192.jpg src\help
copy help\help_html_m690fb192.png src\help
copy help\help_html_m52578ce1.png src\help

mkdir src\data
copy data\style.css src\data
copy data\style-w.css src\data

mkdir src\data\bibles
copy data\bibles\Bible_Structure.txt src\data\bibles
copy data\bibles\list-u.txt src\data\bibles

mkdir src\images
copy images\Logo_2a.png src\images
copy images\Logo_2a.psd src\images
copy images\Logo_ikon_3.psd src\images
copy images\Logo_ikon_3ico.ico src\images
copy images\mac1.icns src\images
copy images\win1.ico src\images

mkdir src\msi
copy msi\CDBible5.wxs src\msi

mkdir src\update
copy update\update.pro src\update
copy update\update_bg.qm src\update
copy update\update_bg.ts src\update
copy update\mainWindow.cpp src\update
copy update\mainWindow.h src\update
copy update\update.cpp src\update

pause

cd src
"Qt-4.6.3-L.bat"
