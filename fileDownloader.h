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

#include <QFile>
#include <QProcess>
#include <QNetworkReply>

class QProgressBar;
class QPushButton;

// Обект извършващ изтегляне на файлове от Интернет.
// Показва напредъка на изтеглянето варху QProgressBar лента.
// С щракваане върху QPushButton изтеглянето може да се прекъсне.
class FileDownloader: public QObject
{
  Q_OBJECT

public:
  FileDownloader( QObject *parent=0, QProgressBar *pbar=0, QPushButton *button=0 );
  
  bool notDone;

  QNetworkReply *reply;

  // Изтегляне от адрес of и запис във файл lf
  void downloadFile(const QString &of, const QString &lf);
  // Изтегляне на zip файл с адрес of, запис във файл lf и разархивиране
  // ms - съобщение, което се показва след разархивирането
  void downloadAndUnzip(const QString &of, const QString &lf, const QString &ms = QString());

private slots:
    void onDataReadProgress(qint64 d, qint64 t);
    void onReadyRead();
    void onRequestFinished();
    void onDownloadDone(bool e);
    void onUnzipFinished(int ec, QProcess::ExitStatus es);

private:
    bool doUnzip = false;
    QProgressBar *progressBar;
    QPushButton *pushButton;
    QFile *file;
    int getID;
    QString doneMessage, zipFile, zipDir;
    QProcess *zipProcess;
    void unzipStep(); // Разархивиране на изтеглен zip файл
};

QString progDir();
void setProgDir();

#endif
