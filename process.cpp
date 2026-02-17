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

#include "process.h"
#include "showMessage.h"

#include <QFileInfo>

MyProcess::MyProcess( QObject * parent)
   :QProcess(parent)
{
   program = "proba";
//   connect(this, SIGNAL(error( QProcess::ProcessError)), this, SLOT(onError( QProcess::ProcessError)));
};

void MyProcess::start(){
   QProcess::start(program,arguments);
};

void MyProcess::start(const QString &pr){
   program = pr;
   QProcess::start(pr, arguments);
};

void MyProcess::browse(const QString &p){
   arguments.clear();
#ifdef Q_WS_WIN
   program = "C:\\Program Files\\Internet Explorer\\iexplore.exe";
   QFileInfo fi(p);
   QString p1=p;
   if (fi.exists()) p1 = fi.absoluteFilePath();
   arguments << p1;
#else
   program = "open";
   arguments << p;
#endif
   start();
};

void MyProcess::edit(const QString &f){
   if (!QFileInfo(f).exists()) {
      showMessage(tr("File %1 don't exist.").arg(f));
      return;
   }
   arguments << f;
#ifdef Q_WS_MAC
   start("open");
#else
  start("notepad");
#endif
};

void MyProcess::onError( QProcess::ProcessError error ){
   QString er;
   switch (error){
   case QProcess::FailedToStart: er = tr("Failed To Start"); break;
   case QProcess::Crashed: er = tr("Crashed"); break;
   case QProcess::Timedout: er = tr("Timedout"); break;
   case QProcess::WriteError: er = tr("Write Error"); break;
   case QProcess::ReadError: er = tr("Read Error"); break;
   case QProcess::UnknownError: er = tr("Unknown Error"); break;
   }
   showMessage(er+" "+program);
};

