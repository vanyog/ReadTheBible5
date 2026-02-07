#include "mainWindow.h"
#include "../showMessage.h"

#include <QProcess>
#include <QFileInfo>
#include <QTimer>

UMW::UMW( QWidget * parent, Qt::WindowFlags flags )
   :QMainWindow(parent, flags)
{
   is_Ok = false;
   QStringList a = qApp->arguments();

   if (a.size()!=3) { showMessage(tr("Incorrect arguments"),tr("CD Bible 5 updater"));  return; }

   process = new QProcess(this);
   connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
   connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onError(QProcess::ProcessError)));

   zipFile = a.at(1);
   al << "-o" << zipFile;

   wd = "";
#ifdef Q_WS_MAC
   wd = QFileInfo(QString( a.at(2) )+"/../../..").absolutePath();
#else
   wd = QFileInfo( a.at(2) ).absolutePath();
#endif
//   showMessage(wd);

   next = a.at(2);

   QTimer::singleShot(2000, this, SLOT(onStartUnzip()));

   is_Ok=true;
};

bool UMW::isOk(){
   return is_Ok;
};

void UMW::onFinished( int exitCode, QProcess::ExitStatus exitStatus ){
   if (exitCode==0){
      QFile(zipFile).remove();
   }
   else {
      showMessage(tr("Unzip Exit Code: %1.<br>Extract files from %2 manualy.").arg(exitCode).arg(zipFile));
      qApp->quit();
   }
   process->setParent(0);
   al.clear();
//   showMessage(next);
   process->start(next,al);
   qApp->quit();
};

void UMW::onError(QProcess::ProcessError error){
   QString er;
   switch (error){
   case QProcess::FailedToStart: er = "Failed To Start"; break;
   case QProcess::Crashed: er = "Crashed"; break;
   case QProcess::Timedout: er = "Timedout"; break;
   case QProcess::WriteError: er = "Write Error"; break;
   case QProcess::ReadError: er = "Read Error"; break;
   case QProcess::UnknownError: er = "Unknown Error"; break;
   }
   showMessage(er+" unzip");
   qApp->quit();
};

void UMW::onStartUnzip(){
   process->setWorkingDirectory(wd);
   process->start("unzip",al);
};


