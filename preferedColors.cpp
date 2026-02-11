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

#include "preferedColors.h"
#include "showMessage.h"

#include <QSettings>

PreferedColor::PreferedColor(QWidget *parent, Qt::WindowFlags f)
 :QDialog(parent,f)
{
   ui.setupUi(this);
   connect(ui.pushButton_2, SIGNAL(pressed()), this, SLOT(onBaseColorPressed()));
   connect(ui.pushButton_3, SIGNAL(pressed()), this, SLOT(onBibleTextColorPressed()));
   connect(ui.pushButton_4, SIGNAL(pressed()), this, SLOT(onActiveVerseColorPressed()));
   connect(ui.pushButton_5, SIGNAL(pressed()), this, SLOT(onFootnoteColorPressed()));
   connect(ui.pushButton_6, SIGNAL(pressed()), this, SLOT(onFoundWordColorPressed()));
};

const QColor &PreferedColor::baseColor() const{
   return ui.lineEdit->palette().color(QPalette::Active,QPalette::Base);
};

const QColor &PreferedColor::bibleTextColor() const{
   return ui.lineEdit_2->palette().color(QPalette::Active,QPalette::Text);
};

const QColor &PreferedColor::activeVerseColor() const{
   return ui.lineEdit_3->palette().color(QPalette::Active,QPalette::Text);
};

const QColor &PreferedColor::footnoteColor() const{
   return ui.lineEdit_4->palette().color(QPalette::Active,QPalette::Text);
};

const QColor &PreferedColor::foundWordColor() const{
   return ui.lineEdit_5->palette().color(QPalette::Active,QPalette::Text);
};

void PreferedColor::setBaseColor(const QColor &c){
  setColorTo(ui.lineEdit, QPalette::Base, c);
  setColorTo(ui.lineEdit_2, QPalette::Base, c);
  setColorTo(ui.lineEdit_3, QPalette::Base, c);
  setColorTo(ui.lineEdit_4, QPalette::Base, c);
  setColorTo(ui.lineEdit_5, QPalette::Base, c);
  emit toChangeBaseColor(c);
};

void PreferedColor::setBibleTextColor(const QColor &c){
  setColorTo(ui.lineEdit, QPalette::Text, c);
  setColorTo(ui.lineEdit_2, QPalette::Text, c);
  emit toChangeTextColor(c);
};

void PreferedColor::setActiveVerseColor(const QColor &c){
  setColorTo(ui.lineEdit_3, QPalette::Text, c);
  emit toChangeOtherTextColor();
};

void PreferedColor::setFootnoteColor(const QColor &c){
  setColorTo(ui.lineEdit_4, QPalette::Text, c);
  emit toChangeOtherTextColor();
};

void PreferedColor::setFoundWordColor(const QColor &c){
  setColorTo(ui.lineEdit_5, QPalette::Text, c);
  emit toChangeOtherTextColor();
};

void PreferedColor::onBaseColorPressed(){
  QColor c=QColorDialog::getColor(baseColor(),this,ui.lineEdit->text(),cdOptions());
  setBaseColor(c);
};

void PreferedColor::onBibleTextColorPressed(){
  QColor c=QColorDialog::getColor(bibleTextColor(),this,ui.lineEdit->text(),cdOptions());
  setBibleTextColor(c);
};

void PreferedColor::onActiveVerseColorPressed(){
  QColor c=QColorDialog::getColor(bibleTextColor(),this,ui.lineEdit->text(),cdOptions());
  setActiveVerseColor(c);
};

void PreferedColor::onFootnoteColorPressed(){
  QColor c=QColorDialog::getColor(bibleTextColor(),this,ui.lineEdit->text(),cdOptions());
  setFootnoteColor(c);
};

void PreferedColor::onFoundWordColorPressed(){
  QColor c=QColorDialog::getColor(bibleTextColor(),this,ui.lineEdit->text(),cdOptions());
  setFoundWordColor(c);
};

void PreferedColor::setColorTo(QWidget *le, QPalette::ColorRole r, const QColor &c){
   if (!c.isValid()) return;
   QPalette p = le->palette();
   p.setColor(QPalette::Active, r, c);
   p.setColor(QPalette::Inactive, r, c);
   le->setPalette(p);
};

void PreferedColor::writeSettings(QSettings *s){
   s->setValue("preferedColorGeometry",saveGeometry());
   s->setValue("preferedBaseColor",baseColor());
   s->setValue("preferedBibleTextColor",bibleTextColor());
   s->setValue("preferedActiveVerseColor",activeVerseColor());
   s->setValue("preferedFootnoteColor",footnoteColor());
   s->setValue("preferedFoundWordColor",foundWordColor());
   s->setValue("preferedColorQtDialog",ui.checkBox->isChecked());
};

void PreferedColor::readSettings(QSettings *s){
   restoreGeometry(s->value("preferedColorGeometry").toByteArray());
   setBaseColor(s->value("preferedBaseColor").value<QColor>());
   setBibleTextColor(s->value("preferedBibleTextColor").value<QColor>());
   setActiveVerseColor(s->value("preferedActiveVerseColor").value<QColor>());
   setFootnoteColor(s->value("preferedFootnoteColor").value<QColor>());
   setFoundWordColor(s->value("preferedFoundWordColor").value<QColor>());
   ui.checkBox->setChecked( s->value("preferedColorQtDialog").toBool() );
};

QColorDialog::ColorDialogOptions PreferedColor::cdOptions(){
  if (ui.checkBox->isChecked()) return QColorDialog::DontUseNativeDialog;
  else return QColorDialog::ColorDialogOption();
};
