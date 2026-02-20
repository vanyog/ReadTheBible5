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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "build/ui_mainWindow.h"

class QMdiArea;
class QMdiSubWindow;
class BibleWindow;
class ExportDialog;
class WebUpdater;
class MyProcess;
class History;
class FileDownloader;
class Concordance;
class ConcordanceModel;
class VerseCollection;

class BMainWindow : public QMainWindow
{
   Q_OBJECT

public:
   BMainWindow(QWidget *parent = 0);

   void writeSettings();
   void tileOrCascade();
   bool doNotExec();
   bool openAppFolder();

signals:
   void globalIndexChanged(BibleWindow *bw);
   void scrollToActiveVerse();
   void refreshText();

public slots:
   void onFileAppFolder(); // Отваряне папката с програмата

protected:
   void closeEvent(QCloseEvent *event);

private slots:
   void onFileExportHtml();
   void onFileExportTxt();
   void onFileImportTxt();
   void onFileImportLinks();
   void onBibleAction(QAction *action);
   void onBibleWindowActivated(QMdiSubWindow *w);
   void onBibleWindowClosing(BibleWindow *bw);
   void onBibleWindowDestroyed(QObject *obj);
   void onBibleAnchorClicked(const QUrl &link); // Кликване върху препратка
   void onBookChanged(int i);
   void onChapterChanged(int i);
   void onVerseChanged(int i);
   void onGoNextVerse();
   void onGoPreviousVerse();
   void onGoNextChapter();
   void onGoPreviousChapter();
   void onGoVerseForReadieng();
   void onGoBack();
   void onGoForward();
   void onGoBookList();
   void onGoChapterList();
   void onGoVerseList();
   void onGoRandomVerse();
   void onViewSearchingTool();
   void onViewVerseCollection();
   void onViewStayOnTop();
   void onViewColorPreferences();
   void onViewStyles();
   void onViewBiggerFont();
   void onViewSmallerFont();
   void onDWVChanged(bool b);
   void onDW5Changed(bool b);
   void onWindowsNext();
   void onWindowsTile();
   void onWindowsCascade();
   void onWindowsCloseAll();
   void onWindowsCloseOthers();
   void onWindowsCloseActive();
   void onWindowsCrossBGBible();
   void onWindowsVersion43();
   void onWindowsPreferences();
   void onChangeBaseColor(const QColor &c);
   void onChangeTextColor(const QColor &c);
   void onHelpContent();
   void onHelpReadme();
   void onHelpAboutProgram();
   void onHelpAboutBibleVersion();
   void onHelpWebSite();
   void onHelpCheckForUpdate();
   void onMaxItemsChanged(int i);
   void onVerseClick(BibleWindow *ab, int i);

   void on_actionClean_Restart_triggered();

   private:
   Ui::MainWindow ui;
   QMdiArea *mdiArea;
   QHash<QString, BibleWindow*> bibleWindow;
   // Флагове
   bool doTile, // Дали да се пренаредят прозорците с текст
      activeBibleMaximized, // Дали активната библия е максимизирана на цял екран
      do_Not_Exec // Да се прекрати програмата, ако се избере да се инсталира нова версия
      ;
   QHash<QString, QAction*> bAction;
   ExportDialog *exportDialog;
   WebUpdater *webUpdater;
   MyProcess *myProcess;
   History *history;
   FileDownloader *fileDownloader;
   Concordance *concordance;
   ConcordanceModel *cModel0;
   VerseCollection *verseCollection;
   QString lastVersion;

   void fileExport(const QString &ex);
   BibleWindow *setActiveBibleReference(bool si = false);
   void emitIndexChanged(BibleWindow *cb);
   BibleWindow *openBible(const QString &bv);
   bool isOpened(const QString &bv);
   int windowCount(const QString &bv);
   void readSettings();
   void createBActions();
   void setNumberComboBox(QComboBox *cb, int max, int curr);
   BibleWindow *activeBible();
   void updateNavButtons();
   void updateControls(BibleWindow *ab);
   void checkMaximization();
   void goByIndex(BibleWindow *ab, int i);
   void goByGlobalIndex(int i);
   void setSynchronization(BibleWindow *ab);
   void downloadV43();
   void newVersion();
   void downloadActiveBible();
};

void setIntegfaceLanguage();
QString interfaceLanguage();

#endif

