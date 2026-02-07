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

#include "verseCollection.h"
#include "showMessage.h"

#include <QTreeWidgetItem>

VerseCollection::VerseCollection(QPushButton *b1, QPushButton *b2, QPushButton *b3, QPushButton *b4, 
  QTreeWidget *t, QWidget * parent
) : QObject(parent)
{
  bNew   = b1;
  bClear = b2;
  bEdit  = b3;
  bFinish= b4;
  tree = t;
  selItem = 0;
  connect(bNew,   SIGNAL(clicked()), this, SLOT(onNew())   );
  connect(bClear, SIGNAL(clicked()), this, SLOT(onClear()) );
  connect(bEdit,  SIGNAL(clicked()), this, SLOT(onEdit())  );
  connect(bFinish,SIGNAL(clicked()), this, SLOT(onFinish()));
};

void VerseCollection::writeSettings(){
};

void VerseCollection::onClear(){
tree->clearSelection();
};

void VerseCollection::onEdit(){
  if (selItem) tree->closePersistentEditor(selItem,col);
  selItem = tree->currentItem();
  col = tree->currentIndex().column();
  tree->openPersistentEditor(tree->currentItem(),tree->currentIndex().column());
};

void VerseCollection::onFinish(){
  if (selItem){
     tree->closePersistentEditor(selItem,col);
    selItem = 0;
  }
};

void VerseCollection::onNew(){
  static int i = 0;
  QTreeWidgetItem *n;
  if (tree->selectedItems().size())
    n = new QTreeWidgetItem(tree->selectedItems().at(0));
  else
    n = new QTreeWidgetItem(tree);
  i++;
  n->setText(0,tr("New collection %1").arg(i));
};