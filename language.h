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

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QString>

/* Този обект осигурява някои зависещи от езика функции,
   например: връща думите "глава" и "псалм" на различни езици. */

class Language: public QObject
{
   Q_OBJECT

public:
   Language();

   QString chapter(const QString &ln); // Думата "Глава" на език ln
   QString psalm(const QString &ln);      // Думата "Псалм" на език ln
   QString characters(const QString &ln); // Сринг от всички букви на език ln

};

Language *languageObject();

#endif

