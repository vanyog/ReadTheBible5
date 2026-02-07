#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#include <QApplication>
#include <QMainWindow>
#include <QStringList>
#include <QProcess>

class UMW: public QMainWindow
{
   Q_OBJECT

public:
   UMW( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
   
   bool isOk();

private slots:
   void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
   void onError(QProcess::ProcessError error);
   void onStartUnzip();

private:
   QProcess *process;
   QStringList al;
   bool is_Ok;
   QString zipFile, next, wd;
};


#endif
