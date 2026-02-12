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

// Клас, с който се прави проверка дали има и се изтагля нова версия на програмата

#ifndef WEBUPDATER_H
#define WEBUPDATER_H

#include <QFile>

class FileDownloader;
class QProgressBar;
class QPushButton;

class WebUpdater: public QObject
{
  Q_OBJECT

public:
  WebUpdater( const QString & hostName, QObject *parent=0, QProgressBar *pbar=0, QPushButton *buttn=0 );

  bool notOK;
  
  void checkForUpdates(const QString &pth, const QString &cv);
  QString downloadedFile();
  bool updateFromFile(const QString &fn);
  
  void setHost(const QString & hostName);

signals:
  void toUpdate(); // Излъчва се за да запачне обновяването с нова версия

private slots:
  void onReadyRead(/*const QHttpResponseHeader &resp*/);
  void onRequestFinished(int id, bool error);
  void onToUpdate();
  void onUDownloadDone(bool error);
//  void onCheckDone(bool error);
//  void onError();

private:
  int getID;
  QString host, // хост на който се проверява за нова версия на програмата
    path,       // път върху сървъра
    version,   // текуща версия на програмата
    system,   // Операционна система "win", "mac" или "" - ако няма изпалними файлове за системата
    zipFile, dFile,
    infoText; // Текс с информация за неличната на сървъра версия за сваляне
  FileDownloader *fileDownloader;
  QProgressBar *progressBar;
  QPushButton *button;
};


#endif
