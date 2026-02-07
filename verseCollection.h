/*
CD Bible 5 - open source biblical program
Copyright (C) 2012 Vanyo Georgiev <info@vanyog.com>

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

#ifndef verseCollection_H
#define verseCollection_H

#include <QDockWidget>
#include <QPushButton>
#include <QTreeWidget>

// Клас VerseCollection капсулира функционалността, свързана с колекциите от стихове.
// Необходимите widget-и се създават с Qt designer и се подават на класа VerseCollection в конструктора му.

class VerseCollection: public QObject
{
  Q_OBJECT

public:
  VerseCollection(QPushButton *b1, QPushButton *b2, QPushButton *b3, QPushButton *b4, QTreeWidget *t, QWidget * parent = 0);

  void writeSettings(); // Записване на промените

private slots:
  void onNew();
  void onClear();
  void onEdit();
  void onFinish();

private:
  QPushButton *bNew,*bClear,*bEdit,*bFinish;
  QTreeWidget *tree;
  QTreeWidgetItem *selItem;
  int col;
};


#endif