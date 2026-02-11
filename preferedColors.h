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

#ifndef PREFEREDCOLORS_H
#define PREFEREDCOLORS_H

#include "ui_preferedColors.h"

#include <QPalette>
#include <QColorDialog>

class  QSettings;

class PreferedColor: public QDialog
{
  Q_OBJECT

public:
  PreferedColor(QWidget *parent = 0, Qt::WindowFlags f=Qt::WindowFlags());
  const QColor &baseColor() const;
  const QColor &bibleTextColor() const;
  const QColor &activeVerseColor() const;
  const QColor &footnoteColor() const;
  const QColor &foundWordColor() const;
  void setBaseColor(const QColor &c);
  void setBibleTextColor(const QColor &c);
  void setActiveVerseColor(const QColor &c);
  void setFootnoteColor(const QColor &c);
  void setFoundWordColor(const QColor &c);

   void setColorTo(QWidget *le, QPalette::ColorRole r, const QColor &c);
   void writeSettings(QSettings *s);
   void readSettings(QSettings *s);

signals:
  void toChangeBaseColor(const QColor &c);
  void toChangeTextColor(const QColor &c);
  void toChangeOtherTextColor();

private slots:
  void onBaseColorPressed();
  void onBibleTextColorPressed();
  void onActiveVerseColorPressed();
  void onFootnoteColorPressed();
  void onFoundWordColorPressed();

private:
   Ui::PreferedColors ui;
   QColorDialog::ColorDialogOptions cdOptions();
};


#endif
