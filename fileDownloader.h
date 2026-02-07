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

// Клас за обект, който тегли файл от Интернет
// и го разархивира

#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QHttp>
#include <QFile>
#include <QProcess>

class QProgressBar;
class QPushButton;

class FileDownloader: public QHttp
{
  Q_OBJECT

public:
  FileDownloader( QObject *parent=0, QProgressBar *pbar=0, QPushButton *button=0 );
  
  bool notDone;

  void downloadFile(const QString &of, const QString &lf);
  void downloadAndUnzip(const QString &of, const QString &lf, const QString &ms = QString());

private slots:
   void onDataReadProgress(int d, int t);
//   void onReadyRead(const QHttpResponseHeader &resp);
   void onRequestFinished(int id, bool error);
   void onDownloadDone(bool e);
   void onUnzipFinished(int ec, QProcess::ExitStatus es);

private:
  QProgressBar *progressBar;
  QPushButton *pushButton;
  QFile *file;
  int getID;
  QString doneMessage, zipFile;
  QProcess *zipProcess;
};

QString progDir();
void setProgDir();

#endif
