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

#include "myListView.h"
#include "showMessage.h"

MyListView::MyListView(QWidget *parent)
:QListView(parent)
{
// Създаване на изскачащо меню
   ppm = new QMenu(this);
   a1 = new QAction(tr("Copy list"),this);
   a2 = new QAction(tr("Copy verses"),this);
   ppm -> addAction(a1);
   ppm -> addAction(a2);
   connect(a1, SIGNAL(triggered()), this, SLOT(onCopyList()));
   connect(a2, SIGNAL(triggered()), this, SLOT(onCopyVerses()));
};

QModelIndexList MyListView::selectedIndexes(){
   return QListView::selectedIndexes();
};

// Предефинирането на събитието keyPressEvent осигурява смяна на показвания стих
// както става при щрекване върху стиха в списъка

void MyListView::keyPressEvent(QKeyEvent *event){
  QListView::keyPressEvent(event);
  if ( selectedIndexes().size() && ((event->key()==Qt::Key_Up)||(event->key()==Qt::Key_Down)) ){
    emit clicked(selectedIndexes().at(0));
  }
};

// Предефинирането на contextMenuEvent осигурява показване на контекстното меню

void MyListView::contextMenuEvent(QContextMenuEvent *event){
  QListView::contextMenuEvent(event);
  ppm -> exec(QCursor::pos());
};

// Сигнал, който се изпълнява при щракване върху "Копиране на препратките" в контекстното меню
void MyListView::onCopyList(){
  emit popupClicked(0);
};

// Сигнал, който се изпълнява при щракване върху "Копиране на стиховете" в контекстното меню
void MyListView::onCopyVerses(){
  emit popupClicked(1);
};

