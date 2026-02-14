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
#include <QNetworkRequest>
#include <QNetworkReply>

#include <JlCompress.h>

QNetworkAccessManager *netAManager = 0;

FileDownloader::FileDownloader( QObject * parent, QProgressBar *pbar, QPushButton *button)
   :QObject(parent)
{
  progressBar=pbar; pushButton=button;
  file=0; getID=0; notDone=false; doneMessage=""; zipFile=""; zipProcess=0;
  if (!netAManager) netAManager = new QNetworkAccessManager;
};

void FileDownloader::downloadFile(const QString &of, const QString &lf){
    zipFile = lf;
    QUrl url(of);
    QNetworkRequest req(url);
    reply = netAManager->get(req);
    QFileInfo info(lf);
    zipDir = info.absoluteDir().path();
    QDir().mkdir(zipDir);
    file = new QFile(lf);
    if(!file->open(QIODevice::WriteOnly)){
        showMessage(tr("Error opening file %1 for writting").arg(lf));
        return;
    }
    notDone=true;
    progressBar->setValue(0);
    progressBar->setVisible(true);
    pushButton ->setVisible(true);
    connect(reply, &QNetworkReply::readyRead,this,&FileDownloader::onReadyRead);
    connect(reply, &QNetworkReply::downloadProgress,this, &FileDownloader::onDataReadProgress);
    connect(reply,&QNetworkReply::finished,this,&FileDownloader::onRequestFinished);
};


void FileDownloader::downloadAndUnzip(const QString &of, const QString &lf, const QString &ms){
   doneMessage=ms;
   downloadFile(of, lf);
   doUnzip = true;
};

// Показва напредъка на изтеглянето върху лента за напредък
void FileDownloader::onDataReadProgress(qint64 d, qint64 t){
    progressBar->setValue(progressBar->maximum()*t/d);
};

// Записване на пристигащите данни в отворения файл
void FileDownloader::onReadyRead(){
    file->write(reply->readAll());
};

// Изпълнява се след края на изтеглянето
void FileDownloader::onRequestFinished(){
    file->close();
    progressBar->setVisible(false);
    pushButton->setVisible(false);
    if(doUnzip) unzipStep();
};


// Функция, написана от ChatGPT: извличане на ZIP и презаписване на съществуващи файлове
bool extractDirOverwrite(const QString &zipFile, const QString &destDir) {
    // Получаваме списък на файловете в архива
    QStringList files = JlCompress::getFileList(zipFile);
    for (const QString &fileName : files) {
        QString outPath = destDir + "/" + fileName;
        QFile outFile(outPath);
        if (outFile.exists()) {
            if (!outFile.remove()) {
                showMessage( QApplication::tr("Failed to remove existing file:").arg(outPath) );
                return false; // спираме при грешка
            }
        }
        // Уверяваме се, че всички поддиректории съществуват
        QFileInfo fi(outPath);
        QDir().mkpath(fi.path());
    }
    // Извличаме всички файлове
    JlCompress::extractDir(zipFile, destDir);
    return true;
}

void FileDownloader::unzipStep(){
    extractDirOverwrite(zipFile, zipDir);
    QFile z(zipFile);
    z.remove();
    showMessage( tr("File has been downloaded and unziped. Now you can open %1.").arg(doneMessage) );
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
