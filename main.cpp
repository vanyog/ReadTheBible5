/*
CD Bible 5 - open source biblical program
Copyright (C) 2008  Vanyo Georgiev <info@vanyog.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <QApplication>
#include <QTranslator>

#include <QPixmap>
#include <QSplashScreen>
#include <QFileInfo>
#include <QSettings>

#include "mainWindow.h"
#include "showMessage.h"
#include "fileDownloader.h"

int main(int argc, char *argv[])
{
//   QApplication::setStyle(new QWindowsStyle);
   QApplication app(argc,argv);

   setIntegfaceLanguage();
   setProgDir();
   
   QTranslator translator;
   translator.load(progDir()+"Bible_"+interfaceLanguage());
   app.installTranslator(&translator);

   QPixmap pm("images/Logo_2a.png");
   QSplashScreen spls(pm);
   spls.show();
   app.processEvents();

   BMainWindow window;
   if (window.doNotExec()){
        window.onFileAppFolder();
        return 0;
   }
   window.show();
   spls.raise();
   window.tileOrCascade();
   spls.finish(&window);
   
    int i = app.exec();
    if(i==42){
        QProcess::startDetached(QCoreApplication::applicationFilePath(), QCoreApplication::arguments());
        return 0;
    }
    if (window.openAppFolder()) window.onFileAppFolder();
    return i;
}

