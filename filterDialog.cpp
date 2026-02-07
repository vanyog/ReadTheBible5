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

#include <QCheckBox>
#include <QSettings>

#include "filterDialog.h"
#include "concordance.h"
#include "showMessage.h"

FilterDialog::FilterDialog(ConcordanceModel *cm, QWidget *parent, Qt::WindowFlags f)
:QDialog(parent,f)
{
   ui.setupUi(this);
   cModel = cm;
   firstNew = cModel->bible->book(51);
   int bn = cModel->bible->shortTitles.size();
   for(int i=0; i<bn; i++){
     QCheckBox *cb = new QCheckBox(cModel->bible->shortTitles.at(i));
     cb->setChecked(cModel->searchIn.at(i));
     int r = i % 20;
     int c = i / 20;
     ui.gridLayout->addWidget(cb,r,c);
     checkBoxes.append(cb);
   }
   connect(ui.pushButton_2, SIGNAL(pressed()), this, SLOT(onAllBooks()));
   connect(ui.pushButton_3, SIGNAL(pressed()), this, SLOT(onOld()));
   connect(ui.pushButton_4, SIGNAL(pressed()), this, SLOT(onNew()));
   connect(ui.pushButton_5, SIGNAL(pressed()), this, SLOT(onClear()));
   connect(ui.pushButton_6, SIGNAL(pressed()), this, SLOT(onInverse()));
   connect(this, SIGNAL(accepted()), this, SLOT(onAccept()));
};

void FilterDialog::onAllBooks(){
   for(int i=0; i<checkBoxes.size(); i++) checkBoxes.at(i)->setChecked(true);
};

void FilterDialog::onOld(){
   for(int i=0; i<checkBoxes.size(); i++) checkBoxes.at(i)->setChecked(i<firstNew-1);
};

void FilterDialog::onNew(){
   for(int i=0; i<checkBoxes.size(); i++) checkBoxes.at(i)->setChecked(i>firstNew-2);
};

void FilterDialog::onClear(){
   for(int i=0; i<checkBoxes.size(); i++) checkBoxes.at(i)->setChecked(false);
};

void FilterDialog::onInverse(){
   for(int i=0; i<checkBoxes.size(); i++) checkBoxes.at(i)->setChecked(!checkBoxes.at(i)->isChecked());
};

void FilterDialog::onAccept(){
   for(int i=0; i < cModel->searchIn.size(); i++)
      cModel->searchIn[i] = checkBoxes.at(i)->isChecked();
};

