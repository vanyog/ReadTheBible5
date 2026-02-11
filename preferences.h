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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "ui_preferences.h"
#include "preferedColors.h"

class  QSettings;

class Preferences: public QDialog
{
  Q_OBJECT

public:
  Preferences( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );

   void writeSettings(QSettings *s);
   void readSettings(QSettings *s);
   int footnotes();
   int references();
   QString CrossBgBiblePath();
   void showEvent(QShowEvent *event);

signals:
   void optionChanged();
   void maxItemsChanged(int i);
   void fontChaged(QFont font);

private slots:
   void onCurrentIndexChanged(int i);
   void onValueChanged(int i);
   void onBrowse();
   void onFontChange();
   void onColorButtonPressed();
   void onAccepted();

private:
   Ui::preferencesDialog ui;
   int proxyType,port;
   int langIndex;
   
   void setProxy();

};

void createPreferences( QWidget * parent = 0 );
Preferences *preferences();
PreferedColor *preferedColor();

#endif
