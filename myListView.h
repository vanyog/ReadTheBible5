/*
CD Bible 5 - open source biblical program
Copyright (C) 2009  Vanyo Georgiev <info@vanyog.com>

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

#include <QListView>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

/* Дефиниране на клас, наследник на QListView, който се използва за показване
списъка на намерените чрез търсене по думи стихове.
В добавка към QListView този клас има изскачащо меню, при щракване върху което се изпраща сигнал.
Сигналът се приемат от клас Concordance, дефиниран в concordance.h. */

class MyListView: public QListView
{
  Q_OBJECT

public:
  MyListView(QWidget *parent=0);

// Предефинира се само за да се направи public
   QModelIndexList selectedIndexes();
  
signals:
  void popupClicked(int i); // Сигнал, който се изпраща при щракване върху изскачащото меню.
  // i e номера на щракнатия ред от менюто
  
protected:
  void keyPressEvent(QKeyEvent *event);
  void contextMenuEvent(QContextMenuEvent *event);

private slots:
  void onCopyList();
  void onCopyVerses();

private:
  QMenu *ppm;
  QAction *a1;
  QAction *a2;
};
