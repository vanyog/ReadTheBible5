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

// Клас за обект, който извършва проверяването за нова версия,
// изтеглянето и инсталирането й.

#include "webUpdater.h"
#include "showMessage.h"
#include "fileDownloader.h"
#include "process.h"

#include <QApplication>
#include <QStringList>
#include <QFileInfo>
#include <QHttpResponseHeader>
#include <QCloseEvent>

WebUpdater::WebUpdater( const QString & hostName, quint16 port, QObject *parent, QProgressBar *pbar, QPushButton *buttn  )
   :QHttp(hostName, port, parent)
{
   getID = 0;
   host = hostName;
   path = "";
   version = "";
   progressBar = pbar;
   button = buttn;
   system = ""; notOK = false;
#ifdef Q_WS_WIN
   system = "win";
#endif
#ifdef Q_WS_MAC
   system = "mac";
#endif
   zipFile = ""; 
   dFile="";
   fileDownloader = 0;
   connect(this, SIGNAL(readyRead(const QHttpResponseHeader&)), this, SLOT(onReadyRead(const QHttpResponseHeader&)));
   connect(this, SIGNAL(requestFinished(int,bool)), this, SLOT(onRequestFinished(int,bool)));
//   connect(this, SIGNAL(done(int,bool)), this, SLOT(onCheckDone(int,bool)));
};

//
// Стартира проверката за нова версия
// pth - директория на сървъра с файловете за теглене 
// cv - версия на програмата, това е стойността на променлива progVersion от mainWindow.cpp
//
void WebUpdater::checkForUpdates(const QString &pth, const QString &cv){
   // Ако стрингът system е празен, значи няма двоични файлове за текущата операционна система
   if (!system.size()) {
      showMessage( tr("There is no binary updates for your operating system. Visit the program web site for more detales.") );
      return;
   }
   // Път до файла на сървъра с информация за най-новата версия, налична там
   QString vf = pth+"version1-"+system+".txt";
   notOK = false; // Флаг за неуспешна проверка. Ще стане true в случай на неуспех.
   // Четене на файла с информация от сървъра. Щом сървърът отговори се изпълнява слота onRequestFinished(int,bool)
   getID = get(vf);
   path = pth;
   version = cv;
};

//
// Връща името на изтегления файл
//
QString WebUpdater::downloadedFile(){
   return dFile;
};

//
// Съобщава, че е изтеглен zip файл fn с нова версия
//
bool WebUpdater::updateFromFile(const QString &fn){
  if (!fn.size()) return false;
  if ( !QFileInfo(fn).exists() ) return false;
   dFile = fn;
   // Ако се отговори, да не се прави сега актуализирането, а по-късно,
   // dFile ще продължи да съдържа името на изтегления файл и 
   // при затваряне на програмата то ще се запази в настройките.
   // Следващото стартиране на програмата ще открие това име и ще предложи отново да се направи актуализиране.
   if (yesNo( tr("New version has been downloaded. Would you like to quit now and load it? (Click NO to do this later.)"),
       tr("CD Bible 5") )==NO) return false;
   showMessage(tr("<p>After you click OK button this program will be closed and the containing folder will be opened. Please</p><ol><li>Delete the executable file</li><li>unzip the new file from the zip archive</li><li>delete the zip file.</li></ol>"));
   // Прекратяване на настоящата програма.
   // Преди да бъде прекратена се изпълнява closeEvent, която извиква writeSetting.
   // Това запазва името на изтегления файл, за да се извърши актуализиране при следващо стартиране.
   QApplication::sendEvent(parent(),new QCloseEvent());
   QApplication::exit();
   return true;
};

//
// Задава име на сървъра за теглене
//
void WebUpdater::setHost(const QString & hostName){
   host = hostName;
};

//
// Слот, който се извиква щом има данни за четене от сървъра
//
void WebUpdater::onReadyRead(const QHttpResponseHeader &resp){
    Q_UNUSED(resp);
//   showMessage("ReadyRead");
   int sc = 0; // Код на отговора от сървъра. 200 - успешен отговор
   QHttpResponseHeader rh = lastResponse();
   if (rh.isValid()) sc =rh.statusCode();
   if (sc!=200){
      showMessage( tr("Not OK response from server.\nError: %1.").arg(sc) );
      notOK = true;
      return;
   }
   emit onToUpdate(); // Странно!
};

// Слот, който се извиква при завършване на http заявката

void WebUpdater::onRequestFinished( int id, bool error){
   Q_UNUSED(id);
   if (error){
      showMessage( tr("An error occur trying to check for update:\n%1").arg(errorString()) );
      return;
   }
};

//
// Изпълнява се, когато има успешен отговор от сървъра и данни за четене
//
void WebUpdater::onToUpdate(){
   QString v = QString( readAll() ); // Прочитат се всички данни от сървъра - съдържанието на файла с информация за нова версия
   QStringList vl = v.split("\n");
   if (vl.size()<2) return;
   // Първият ред от файла с информация е номера на новата версия
   // Ако този номер съвпада с настоящата версия - съобщение и край
   if (version==vl.at(0).trimmed()){
     showMessage( tr("The program is up to date.") );
     return;
   }
   // Въпрос дали да се изтегли
   if ( yesNoCancel(tr("There is a new version. Would you like to download it?"))!=YES ) return;
   // Вторият ред е името на zip файла
   zipFile = vl.at(1).trimmed();
   // Ако има трети ред, той е номер на версията на Qt
   // Ако тази версия е различна от настоящата името на zip файла се взема от четвъртия ред 
   if (vl.size()>3){
     if (vl.at(2).trimmed()!=QString(qVersion())) zipFile=vl.at(3);
   }
   QString of = "http://"+host+path+zipFile; // URL на zip файла
   if (!fileDownloader){
      fileDownloader = new FileDownloader(this,progressBar,button);
      connect(fileDownloader, SIGNAL(done(bool)), this, SLOT(onUDownloadDone(bool)));
   }
   // Изтегляне на zip файла
   fileDownloader->downloadFile( of, progDir() + zipFile );
};

/*void WebUpdater::onCheckDone(bool error){
   if (error){
      showMessage( tr("An error occur trying to check for update:\n%1").arg(errorString()) );
      return;
   }
   showMessage("Done");
};*/

//
// Изпълнява се щом fileDownloader завърши изтеглянето на zip файла
//
void WebUpdater::onUDownloadDone(bool error){
   if (error) return;
   fileDownloader = 0;
   if (notOK) return;
   // Изпълняване на актуализирането
   updateFromFile( QFileInfo(zipFile).absoluteFilePath() );
};

