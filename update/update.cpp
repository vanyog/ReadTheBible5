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

// Главен файл на помощна програма, която извършва актуализирането до по-нова версия
// В момента не функционира добре, трябва да и се оправят бъговете

#include <QTranslator>
#include <QLocale>

#include "../showMessage.h"
#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    QTranslator translator;
    translator.load("update_"+QLocale::system().name());
    app.installTranslator(&translator);

    UMW w; // Главен прозорец, дефиниран в mainWindow.h
    
    // Ако актуализирането е протекло успешно, програмата завършва
    if (!w.isOk()) return 0;
    
    // В противен случай се показва главния прозорец
    w.show();
    return app.exec();
}

