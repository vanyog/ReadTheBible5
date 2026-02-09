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

// Клас, обект за теглене на файлове

#include "fileDownloader.h"
#include "showMessage.h"

#include <QUrl>
#include <QProgressBar>
#include <QPushButton>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QNetworkAccessManager>

QNetworkAccessManager *netAManager = 0;

FileDownloader::FileDownloader( QObject * parent, QProgressBar *pbar, QPushButton *button)
   :QObject(parent)
{
  progressBar=pbar; pushButton=button;
  file=0; getID=0; notDone=false; doneMessage=""; zipFile=""; zipProcess=0;
  if (!netAManager) netAManager = new QNetworkAccessManager;
  connect(this, SIGNAL(dataReadProgress(int,int)), this, SLOT(onDataReadProgress(int,int)));
  connect(this, SIGNAL(requestFinished(int,bool)), this, SLOT(onRequestFinished(int,bool)));
//  connect(this, SIGNAL(readyRead(const QHttpResponseHeader&)), this, SLOT(onReadyRead(const QHttpResponseHeader&)));
};

void FileDownloader::downloadFile(const QString &of, const QString &lf){
   QUrl url(of);
   QNetworkRequest req(url);
   QDir dir( QFileInfo(lf).absolutePath() );
   if (!dir.exists()) dir.mkpath(dir.path());
   file=new QFile(lf);
   if (file->open(QIODevice::WriteOnly)){
      replay = netAManager->get(req);
 //     getID=get(url.path(),file);
      notDone=false;
   }
   else showMessage( tr("Cannot write to file: %1.\n%2.").arg(lf).arg(file->errorString()) );
};

void FileDownloader::downloadAndUnzip(const QString &of, const QString &lf, const QString &ms){
   doneMessage=ms;
   disconnect(this, SIGNAL(done(bool)), 0, 0);
   connect(this, SIGNAL(done(bool)), this, SLOT(onDownloadDone(bool)));
   zipFile = lf;
   downloadFile(of, lf);
};

/*void FileDownloader::onReadyRead(const QHttpResponseHeader &resp){
   showMessage("ReadyRead");
   int sc = 0;
   QHttpResponseHeader rh = lastResponse();
   if (rh.isValid()) sc =rh.statusCode();
   if (sc!=200){
      showMessage( tr("Not OK response from server.\n Error: %1.").arg(sc) );
      return;
   }
};*/

void FileDownloader::onDataReadProgress(int d, int t){
/*   int sc = 0;
   QHttpResponseHeader rh = lastResponse();
   if (rh.isValid()) sc =rh.statusCode();
   if (sc!=200){
      notDone=true;
      showMessage( tr("Not OK response from server.\nError: %1.").arg(sc) );
      return;
   }
   if (progressBar) {
      progressBar->show();
      progressBar->setMaximum(t);
      progressBar->setValue(d);
   };*/
};

void FileDownloader::onRequestFinished(int id, bool error){
/*   if (progressBar)  progressBar->hide();
   if (pushButton)  pushButton->hide();
   if ((id==getID) && file) file->close();
   if (error) showMessage( tr("Error: %1.\nCheck your internet connection.").arg(errorString()) );
   if (id==getID) file=0;*/
};

void FileDownloader::onDownloadDone(bool e){
   if (e || notDone) return;
   QStringList a;
   a << "-o" << QFileInfo(zipFile).absoluteFilePath();
   if (!zipProcess){
      zipProcess = new QProcess(this);
      disconnect(zipProcess, SIGNAL(finished(int,QProcess::ExitStatus)), 0, 0);
      connect(zipProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onUnzipFinished(int,QProcess::ExitStatus)));
   }
   zipProcess->setWorkingDirectory(QFileInfo(zipFile).absolutePath());
   zipProcess->start("unzip",a);
};

void FileDownloader::onUnzipFinished(int ec, QProcess::ExitStatus es){
   Q_UNUSED(ec); Q_UNUSED(es);
   showMessage(tr("File has been downloaded and unziped. Now you can open %1.").arg(doneMessage));
};

// Променлива, която запомня абсолютната директория, в която се намира програмата
QString prog_Dir;

// Връща абсолютната директория, в която се намира програмата
QString progDir(){
    return prog_Dir;
};

// Инициализира променливата, която съхранява абсолютната директория на програмата
// или текущата директория
void setProgDir(){
    prog_Dir = qApp->applicationFilePath();
    int l = QDir(prog_Dir).dirName().size();
    Q_UNUSED(l);
#ifdef Q_WS_MAC
    prog_Dir = prog_Dir.left(prog_Dir.size()-2*l-20);
    Q_UNUSED(l);
#else
    prog_Dir = QDir::currentPath()+"/";
#endif
};
