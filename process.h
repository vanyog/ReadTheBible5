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

#ifndef PROCESS_H
#define PROCESS_H

#include <QProcess>

class MyProcess: public QProcess
{
   Q_OBJECT

public:
   MyProcess( QObject * parent = 0 );
   
   void start();
   void start(const QString &pr);
   void browse(const QString &p);
   void edit(const QString &f);

private slots:
   void onError( QProcess::ProcessError error );

private:
   QString program;
   QStringList arguments;
};


#endif
