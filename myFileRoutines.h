/*
VGEdit - open source programer's editor
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

// Някои функции, улесняващи работата с текстови файлове

#ifndef MYFILERUTINES_H
#define MYFILERUTINES_H

#include <QStringList>

// Връща списък от имена на другите файловете от същата директория с файла fn
QStringList otherFiles(const QString &fn);

// Връща стринг, съдържащ целия файл fn с ковировка codec
QString fileContent(const QString &fn, const QString &codec = "cp1251");

// Записва във файл fn стринга fc с кодировка codec
void saveToFile(const QString &fn, const QString &fc);

// Връща истина ако файл fn съдържа стринг s
bool fileContains(const QString &fn, const QString &s, Qt::CaseSensitivity cs = Qt::CaseInsensitive);

#endif
