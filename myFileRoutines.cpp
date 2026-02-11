/*
VGEdit - open source programer's editor
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

#include "myFileRoutines.h"
#include "myDecode.h"
#include "showMessage.h"

#include <QString>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
//#include <QTextCodec>
#include <QStringConverter>


void removeExt(const QString &ex, QStringList *list){
   int c = list->size()-1;
   for(int i=c; i>=0; i--){
      if (list->at(i).endsWith("."+ex) ){
         list->removeAt(i);
      }
   };
};

QStringList otherFiles(const QString &fn){
   QString pn = QFileInfo(fn).absolutePath();
   QDir dir(pn);
   QStringList list = dir.entryList(QDir::Files);
   int i = list.indexOf(QFileInfo(fn).fileName());
   list.removeAt(i);
   removeExt("o",&list);
   return list;
};

QString fileContent(const QString &fn, const QString &codec){
   QFile file(fn);
   if (!file.open(QFile::ReadOnly | QFile::Text)) {
       showMessage( QApplication::tr("Cannot read file:\n%1.\n%2.").arg(fn,file.errorString()) );
      return "";
   }
   QByteArray r = file.readAll();
   file.close();
   QString s = myDecode(r,codec);
   QApplication::restoreOverrideCursor();
   return s;
};

void saveToFile(const QString &fn, const QString &fc, const QString &codec){
    Q_UNUSED(codec)
   QFile file(fn);
   if (!file.open(QFile::WriteOnly | QFile::Text)) {
      showMessage(QApplication::tr("Cannot write to file: %1.\n%2.").arg(fn,file.errorString()));
      return;
   }
   QTextStream in(&file);
   QApplication::setOverrideCursor(Qt::WaitCursor);
   in << fc;
   file.close();
   QApplication::restoreOverrideCursor();
};

bool fileContains(const QString &fn, const QString &s, Qt::CaseSensitivity cs){
  QString fc = fileContent(fn);
  return fc.contains(s,cs);
};
