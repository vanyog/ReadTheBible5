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
#include "DialogUtils.h"

#include <QSettings>
#include <QScreen>

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

QColor PreferedColor::baseColor() const{
    QColor rez = ui.lineEdit->palette().color(QPalette::Base);
    return rez;
};

QColor PreferedColor::bibleTextColor() const{
   QColor rez = ui.lineEdit_2->palette().color(QPalette::Active,QPalette::Text);
    return rez;
};

QColor PreferedColor::activeVerseColor() const{
    auto p = ui.lineEdit_3->palette();
    QColor rez = p.color(QPalette::Active, QPalette::Text);
    return rez;
};


QColor PreferedColor::footnoteColor() const{
   QColor rez = ui.lineEdit_4->palette().color(QPalette::Active,QPalette::Text);
    return rez;
};

QColor PreferedColor::foundWordColor() const{
   QColor rez = ui.lineEdit_5->palette().color(QPalette::Active,QPalette::Text);
    return rez;
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

QColor myGetColor(QColor initial, QWidget *parent, QString title, QColorDialog::ColorDialogOptions options)
{
    QColor c = initial;
    QColorDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setOptions(options);
    ensureDialogFitsScreen(&dlg, true);
    dlg.setCurrentColor(initial);
    if (dlg.exec() == QDialog::Accepted) {
        c = dlg.selectedColor();
    }
    return c;
}

void PreferedColor::onBaseColorPressed(){
  QColor c = myGetColor(baseColor(), this, ui.lineEdit->text(), cdOptions() );
  setBaseColor(c);
};

void PreferedColor::onBibleTextColorPressed(){
  QColor c = myGetColor(bibleTextColor(),this,ui.lineEdit_2->text(),cdOptions());
  setBibleTextColor(c);
};

void PreferedColor::onActiveVerseColorPressed(){
  QColor c = myGetColor(activeVerseColor(),this,ui.lineEdit_3->text(),cdOptions());
  setActiveVerseColor(c);
};

void PreferedColor::onFootnoteColorPressed(){
  QColor c = myGetColor(footnoteColor(),this,ui.lineEdit_4->text(),cdOptions());
  setFootnoteColor(c);
};

void PreferedColor::onFoundWordColorPressed(){
  QColor c = myGetColor(foundWordColor(),this,ui.lineEdit_5->text(),cdOptions());
  setFoundWordColor(c);
};

QColor adaptColorForDarkMode(const QColor &c)
{
    if (!c.isValid()) return c;
    // Конвертиране в HSL
    float h, s, l, a;
    c.getHslF(&h, &s, &l, &a);
    // Инверсия на светлината
    l = 1.0 - l;
    // Малка корекция – иначе стават прекалено "кисели"
    l = std::clamp(l * 0.85 + 0.1, 0.0, 1.0);
    // По желание: леко намаляване на saturation
    s *= 0.9;
    QColor result;
    result.setHslF(h, s, l, a);
    return result;
}

bool isDarkMode()
{
    QColor bg = qApp->palette().color(QPalette::Window);
    return bg.lightness() < 128;
}

void PreferedColor::setColorTo(QWidget *le, QPalette::ColorRole r, const QColor &c)
{
    if (!c.isValid()) return;
    QColor finalColor = c;
    if (isDarkMode()) {
        finalColor = adaptColorForDarkMode(c);
    }
    QPalette p = le->palette();
    p.setColor(QPalette::Active, r, finalColor);
    p.setColor(QPalette::Inactive, r, finalColor);
    le->setPalette(p);
}

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
