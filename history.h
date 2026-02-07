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

#ifndef HISTORY_H
#define HISTORY_H

#include "bibleWindow.h"

#include <QList>
#include <QObject>

class History: public QObject
{
  Q_OBJECT

public:
  History();
  
  void push(int i);
  int back();
  int forward();
  
private slots:

private:
  QList<int> list;
  int c_i, m_i;
};


#endif
