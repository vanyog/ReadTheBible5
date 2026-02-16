/*
CD Bible 5 - open source biblical program
Copyright (C) 2008 Vanyo Georgiev <info@vanyog.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
m
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "mainWindow.h"
#include "myFileRoutines.h"
#include "showMessage.h"
#include "bibleWindow.h"
#include "exportDialog.h"
#include "webUpdater.h"
#include "preferences.h"
#include "process.h"
#include "history.h"
#include "fileDownloader.h"
#include "concordance.h"
#include "verseCollection.h"
#include "ctime"

#include <QMdiArea>
#include <QSettings>
#include <QMdiSubWindow>
#include <QDir>
#include <QThread>
#include <QTimer>
#include <QCloseEvent>
#include <QTime>
#include <QFileDialog>
#include <QRandomGenerator>

QString progVersion = "5.3.0";
QString progURL = "https://vanyog.com/index.php?pid=24&lang=";
QString progEmail = "info@vanyog.com";
#ifdef Q_WS_WIN
   QString styleFile = "style-w.css";
#else
   #ifdef Q_WS_MAC
     QString styleFile = "style.css";
   #else
     QString styleFile = "style-w.css";
   #endif
#endif

BMainWindow::BMainWindow(QWidget *parent)
   : QMainWindow(parent)
{
   ui.setupUi(this);
   ui.progressBar->hide();
   ui.pushButton->hide();
   ui.dockWidget_2->setTitleBarWidget(new QWidget());
   
   setDownloadWidgets(ui.progressBar, ui.pushButton);
   mdiArea = ui.mdiArea;
   doTile = true;
   activeBibleMaximized = false;
   exportDialog = new ExportDialog(this);
   history = new History();
   webUpdater = new WebUpdater( downloadSite(), this, ui.progressBar );
   fileDownloader = 0;
   do_Not_Exec = false;

   concordance = new Concordance(this, ui.comboBox, ui.listView, ui.listView_2, ui.pushButton_2);
   connect(concordance, SIGNAL(verseClicked(BibleWindow*,int)), this, SLOT(onVerseClick(BibleWindow*,int)));
   connect(this, SIGNAL(globalIndexChanged(BibleWindow*)), concordance, SLOT(onGlobalIndexChanged(BibleWindow*)));
   cModel0 = 0;
   
   verseCollection = new VerseCollection(ui.pushButton_5,ui.pushButton_6,ui.pushButton_7,ui.pushButton_8,ui.treeWidget,this);
   
   createPreferences(this);
   connect(preferences(), SIGNAL(maxItemsChanged(int)), this, SLOT(onMaxItemsChanged(int)) );
   connect(preferedColor(), SIGNAL(toChangeBaseColor(const QColor &)), this, SLOT(onChangeBaseColor(const QColor &)));
   connect(preferedColor(), SIGNAL(toChangeTextColor(const QColor &)), this, SLOT(onChangeTextColor(const QColor &)));

   createBActions();
   readGlobalStructure();
   readSettings();
   myProcess = new MyProcess( this );

   connect(ui.actionExport_as_html, SIGNAL(triggered()), this, SLOT(onFileExportHtml()));
   connect(ui.actionExport_as_txt, SIGNAL(triggered()), this, SLOT(onFileExportTxt()));
   connect(ui.actionImport_from_txt, SIGNAL(triggered()), this, SLOT(onFileImportTxt()));
   connect(ui.action_Import_links, SIGNAL(triggered()), this, SLOT(onFileImportLinks()));
   connect(ui.actionOpen_application_folder, SIGNAL(triggered()), this, SLOT(onFileAppFolder()));
   
   if (ui.menu_Bible_1->actions().size())
      connect(ui.menu_Bible_1, SIGNAL(triggered(QAction*)), this, SLOT(onBibleAction(QAction*)));

   connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onBibleWindowActivated(QMdiSubWindow*)));

   connect(ui.comboBox_2, SIGNAL(activated(int)), this, SLOT(onBookChanged(int)));
   connect(ui.comboBox_3, SIGNAL(activated(int)), this, SLOT(onChapterChanged(int)));
   connect(ui.comboBox_4, SIGNAL(activated(int)), this, SLOT(onVerseChanged(int)));

   connect(ui.actionNext_Verse, SIGNAL(triggered()), this, SLOT(onGoNextVerse()));
   connect(ui.actionPrevious_Verse, SIGNAL(triggered()), this, SLOT(onGoPreviousVerse()));
   connect(ui.actionNext_Chapter, SIGNAL(triggered()), this, SLOT(onGoNextChapter()));
   connect(ui.actionPrevious_Chapter, SIGNAL(triggered()), this, SLOT(onGoPreviousChapter()));
   connect(ui.actionVerse_for_reading, SIGNAL(triggered()), this, SLOT(onGoVerseForReadieng()));
   connect(ui.action_Back, SIGNAL(triggered()), this, SLOT(onGoBack()));
   connect(ui.action_Forward, SIGNAL(triggered()), this, SLOT(onGoForward()));
   connect(ui.actionBook, SIGNAL(triggered()), this, SLOT(onGoBookList()));
   connect(ui.actionChapter, SIGNAL(triggered()), this, SLOT(onGoChapterList()));
   connect(ui.actionVerse, SIGNAL(triggered()), this, SLOT(onGoVerseList()));
   connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(onGoBack()));
   connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(onGoForward()));
   connect(ui.actionRandom_Verse, SIGNAL(triggered()), this, SLOT(onGoRandomVerse()));

   connect(ui.action_Searching_toolbox, SIGNAL(triggered()), this, SLOT(onViewSearchingTool()));
   connect(ui.actionVerse_collectien_toolbox, SIGNAL(triggered()), this, SLOT(onViewVerseCollection()));
   connect(ui.actionStay_on_top, SIGNAL(triggered()), this, SLOT(onViewStayOnTop()));
   connect(ui.action_Color_preferences, SIGNAL(triggered()), this, SLOT(onViewColorPreferences()));
   connect(ui.action_Styles, SIGNAL(triggered()), this, SLOT(onViewStyles()));
   connect(ui.actionBigger_font, SIGNAL(triggered()), this, SLOT(onViewBiggerFont()));
   connect(ui.actionSmaller_font, SIGNAL(triggered()), this, SLOT(onViewSmallerFont()));
   
   connect(ui.dockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(onDWVChanged(bool)));
   connect(ui.dockWidget_5, SIGNAL(visibilityChanged(bool)), this, SLOT(onDW5Changed(bool)));

   connect(ui.action_Next_window, SIGNAL(triggered()), this, SLOT(onWindowsNext()));
   connect(ui.action_Tile, SIGNAL(triggered()), this, SLOT(onWindowsTile()));
   connect(ui.action_Cascade, SIGNAL(triggered()), this, SLOT(onWindowsCascade()));
   connect(ui.actionClose_all, SIGNAL(triggered()), this, SLOT(onWindowsCloseAll()));
   connect(ui.actionClose_others, SIGNAL(triggered()), this, SLOT(onWindowsCloseOthers()));
   connect(ui.actionClose_current, SIGNAL(triggered()), this, SLOT(onWindowsCloseActive()));
   connect(ui.actionCrossBGBible, SIGNAL(triggered()), this, SLOT(onWindowsCrossBGBible()));
   connect(ui.actionVersion_4_3, SIGNAL(triggered()), this, SLOT(onWindowsVersion43()));
   connect(ui.action_Preferences, SIGNAL(triggered()), this, SLOT(onWindowsPreferences()));
   connect(ui.action_Preferences_2, SIGNAL(triggered()), this, SLOT(onWindowsPreferences()));

   connect(ui.actionAbout_Program, SIGNAL(triggered()), this, SLOT(onHelpAboutProgram()));
   connect(ui.actionHelp_Content, SIGNAL(triggered()), this, SLOT(onHelpContent()));
   connect(ui.actionReadme_file, SIGNAL(triggered()), this, SLOT(onHelpReadme()));
   connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
   connect(ui.actionAbout_Bible_Version, SIGNAL(triggered()), this, SLOT(onHelpAboutBibleVersion()));
   connect(ui.action_Web_site, SIGNAL(triggered()), this, SLOT(onHelpWebSite()));
   connect(ui.actionCheck_for_updates, SIGNAL(triggered()), this, SLOT(onHelpCheckForUpdate()));
};

bool BMainWindow::doNotExec(){
   return do_Not_Exec;
};

bool BMainWindow::openAppFolder(){
   return (webUpdater->downloadedFile().size()>0);
};

void BMainWindow::closeEvent(QCloseEvent *event){
   writeSettings();
   event->accept();
};

void BMainWindow::fileExport(const QString &ex){
   BibleWindow *bw = activeBible();
   if (!bw) return;
   QString fn = bw->bibleVersion()+ex;
   exportDialog->setFileName(fn);
   exportDialog->setWindowTitle(tr("Export as %1 file").arg(ex));
   if (exportDialog->exec()){
       QString tx="";
      if (ex==".html") tx = bw->toHtml( exportDialog-> whatToExport() );
      else tx = bw->toTxt( exportDialog-> whatToExport() );
      saveToFile(exportDialog->fileName(),tx);
      QSettings s("VanyoG", "CD Bible 5");
      QFileInfo i(exportDialog->fileName());
      s.setValue("eport_dir",i.path());
   };
};

void BMainWindow::onFileExportHtml(){
   fileExport(".html");
};

void BMainWindow::onFileExportTxt(){
   fileExport(".txt");
};

void BMainWindow::onFileImportTxt(){
//   showMessage(tr("This feature is not finished yet. See the source code.")); return;
   BibleWindow *ab = activeBible();
   if (!ab) return;
   QSettings s("VanyoG", "CD Bible 5");
   QString dr = s.value("ImportTxtFile").toString();
   QString fn;
   fn = QFileDialog::getOpenFileName(this, tr("Import Bible from .txt file"), dr, "Text file (*.txt)" );
   if (!fn.size()) return;
   s.setValue("ImportTxtFile",fn);
   ab->import(fn);
};

void BMainWindow::onFileImportLinks(){
   BibleWindow *ab = activeBible();
   if (!ab) return;
   ab->createLinksBin();
};

void BMainWindow::onFileAppFolder(){
   QString d = QApplication::applicationDirPath();
   QString c;
   QProcess *p = new QProcess();
   QStringList a;
#ifdef Q_WS_MAC
   d.append("/../..");
   d = QFileInfo(d).absolutePath();
   c = "open";
   a << d;
#else
 #ifdef Q_WS_WIN
   c = "explorer";
   d = progDir().replace("/","\\");
   a << "/n,/e," << d;
 #else
   showMessage(tr("This is not implemented for your operating system."));
 #endif
#endif
   p->start(c,a);
};

// Изпълнява се при избор на библия от меню "Библия" - "Превод"
void BMainWindow::onBibleAction(QAction *action){
   if (!action) return;
   if (action->objectName()=="actionDownload_again"){
     downloadActiveBible();
     return;
   }
   QString bv = action->objectName();
   if (action->objectName()=="actionOpen_again") bv = lastVersion;

   BibleWindow *bw = openBible(bv);
   if (!bw) return;
   if (!bw->longTitles.size()) action->setChecked(false);
   checkMaximization();
   tileOrCascade();
};

void BMainWindow::onBibleWindowActivated(QMdiSubWindow *w){
    if (!w){ // Затворен е последния прозорец с библия
        ui.comboBox_2->clear();
        ui.comboBox_3->clear();
        ui.comboBox_4->clear();
        return;
   }
   static QMdiSubWindow *w0 = 0; // Статична променлива, която съхранява указатл към последния активиран прозорец
   if (w==w0) return; // Функцията се прекратява ако прозорецът вече е бил активиран
   w0 = w; // Запаметява се последния активиран прозорец
   QString bv = w->widget()->objectName();
   BibleWindow *bw = bibleWindow.value( bv );
   if (bw){
      // Запомня се версията на Библията, за да може да се отвори повторно с "Отвори отново"
      lastVersion = bw->bibleVersion();

      // Поставяне на отметка в меню "Библия"
      QAction *ba = bAction.value(bw->bibleVersion());
      if (ba) ba->setChecked(true);

      // Установяване на променливата synhronize във всеки прозорец с Библия
      setSynchronization(bw);

      // Установяване на конкорданса
      ConcordanceModel *cm = bw->concordance(); // Нов конкорданс
//      concordance->setModel(cm); 
      if (cm!=cModel0)
      { // Ако новия конкорданс е различен от стария
        concordance->setModel(cm); 
        cModel0 = cm;
      }

      // Излъчва се сигнал за смяна активния стих
      emitIndexChanged(bw);

      // Актуализират се падащите списъци
      ui.comboBox_2->clear();
      ui.comboBox_2->addItems( bw->shortTitles );
      if (bw->book()) ui.comboBox_2->setCurrentIndex(bw->book()-1);
      else ui.comboBox_2->setCurrentIndex(-1);
      setNumberComboBox(ui.comboBox_3, bw->chapterCount(), bw->chapter() );
      setNumberComboBox(ui.comboBox_4, bw->verseCount(), bw->verse() );
   }
};

void BMainWindow::onBibleWindowClosing(BibleWindow *bw){
   if (!bw) return;
   QAction *ba = bAction.value(bw->bibleVersion());
   if ( ba && ( windowCount(bw->bibleVersion())<2 ) ) ba->setChecked(false);
   checkMaximization();
};

void BMainWindow::onBibleWindowDestroyed(QObject *obj){
   Q_UNUSED(obj);
   tileOrCascade();
   ui.listView_2->setModel(0);
//   concordance->setModel(0);
};

void BMainWindow::onBibleAnchorClicked(const QUrl &link){
   QString lf = link.fragment();
   int i = link.path().toInt();
   BibleWindow *ab = activeBible();
   if (!ab) return;
   if (lf.size()) ab->setSource("#"+lf);
   else goByIndex( ab, i );
};

void BMainWindow::onBookChanged(int i){
   Q_UNUSED(i);
   BibleWindow *ab = setActiveBibleReference();
   if (!ab) return;
   setNumberComboBox(ui.comboBox_3, ab->chapterCount(), ab->chapter() );
   setNumberComboBox(ui.comboBox_4, ab->verseCount(), ab->verse() );
   setActiveBibleReference(true);
   emitIndexChanged(ab);
};

void BMainWindow::onChapterChanged(int i){
   Q_UNUSED(i);
   BibleWindow *ab = setActiveBibleReference();
   if (!ab) return;
   setNumberComboBox(ui.comboBox_4, ab->verseCount(), ab->verse() );
   setActiveBibleReference(true);
   emitIndexChanged(ab);
};

// Смяна на стиха оп падащия списък
void BMainWindow::onVerseChanged(int i){
   Q_UNUSED(i);
   BibleWindow *ab = setActiveBibleReference(true);
   if (!ab) return;
   emitIndexChanged(ab);
};

// При команда следващ стих
void BMainWindow::onGoNextVerse(){
   BibleWindow *ab = activeBible();
   if (!ab) return;
   int i = ab->verseIndex();
   if (i<ab->verseTotalCount()) i++;
   else showMessage(tr("Last verse is reached %.").arg(ab->verseTotalCount()));
   goByIndex(ab,i);
 //  ui.comboBox_4->setFocus();
};

void BMainWindow::onGoPreviousVerse(){
   BibleWindow *ab = activeBible();
   if (!ab) return;
   int i = ab->verseIndex();
   if (i>1) i--;
   goByIndex(ab,i);
 //  ui.comboBox_4->setFocus();
};

void BMainWindow::onGoNextChapter(){
   BibleWindow *ab = activeBible();
   if (!ab) return;
   int i = ab->verseIndex();
   if (i<ab->verseTotalCount()) i += ui.comboBox_4->count()-ui.comboBox_4->currentIndex();
   else showMessage(tr("Last verse is reached %.").arg(ab->verseTotalCount()));
   goByIndex(ab,i);
   ui.comboBox_4->setFocus();
};

void BMainWindow::onGoPreviousChapter(){
   BibleWindow *ab = activeBible();
   if (!ab) return;
   int i = ab->verseIndex();
   if (i>1) i -= ui.comboBox_4->currentIndex() + 1;
   goByIndex(ab,i);
   ui.comboBox_4->setFocus();
};

void BMainWindow::onGoVerseForReadieng(){
   BibleWindow *ab = activeBible();
   if (!ab) return;
   goByIndex(ab, ab->readPos());
   ui.comboBox_4->setFocus();
};

void BMainWindow::onGoBack(){
    int i = history->back();
    goByGlobalIndex(i);
};

void BMainWindow::onGoForward(){
    int i = history->forward();
    goByGlobalIndex(i);
};

void BMainWindow::onGoBookList(){
  ui.comboBox_2->setFocus(Qt::MouseFocusReason);
  ui.comboBox_2->lineEdit()->selectAll();
};

void BMainWindow::onGoChapterList(){
  ui.comboBox_3->setFocus(Qt::MouseFocusReason);
  ui.comboBox_3->lineEdit()->selectAll();
};

void BMainWindow::onGoVerseList(){
  ui.comboBox_4->setFocus(Qt::MouseFocusReason);
  ui.comboBox_4->lineEdit()->selectAll();
};

void BMainWindow::onGoRandomVerse(){
  BibleWindow *ab = activeBible();
  if (!ab) return;
  static int r = 1;
  if (r) { srand((unsigned)time(0)); r = 0; }
  int i = QRandomGenerator::global()->generate();
//  int i = arc4random();
  i = i % ab->verseTotalCount();
  goByIndex(ab,i);
};

void BMainWindow::onViewSearchingTool(){
//   showMessage(ui.action_Searching_toolbox->isChecked());
   ui.dockWidget->disconnect(SIGNAL(visibilityChanged(bool)));
   ui.action_Searching_toolbox->setChecked(true);
   ui.dockWidget->setVisible( ui.action_Searching_toolbox->isChecked() );
   connect(ui.dockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(onDWVChanged(bool)));
   if (ui.dockWidget->isVisible()) ui.comboBox->setFocus(Qt::TabFocusReason); 
};

void BMainWindow::onViewVerseCollection(){
   ui.dockWidget_5->setVisible( ui.actionVerse_collectien_toolbox->isChecked() );
};

void BMainWindow::onViewStayOnTop(){
  Qt::WindowFlags flags = windowFlags();
  if (ui.actionStay_on_top->isChecked()) flags |= Qt::WindowStaysOnTopHint;
  else flags &= ~Qt::WindowStaysOnTopHint; 
  setWindowFlags(flags);
  show();
};

void BMainWindow::onViewColorPreferences(){
   preferedColor()->exec();
};

void BMainWindow::onViewStyles(){
   MyProcess *p = new MyProcess(this);
   showMessage(tr("Style changes will take effect next time you strat the Bible program. Click OK to start editing."));
   p->edit(styleFile);
};

void changeFontSize(int i){
   QFont font = qApp->font();
   int ps = font.pointSize()+i;
   font.setPointSize(ps);
   qApp->setFont(font);
};

void BMainWindow::onViewBiggerFont(){
   changeFontSize(1);
};

void BMainWindow::onViewSmallerFont(){
   changeFontSize(-1);
};

void BMainWindow::onDWVChanged(bool b){
   ui.action_Searching_toolbox->setChecked(b);
};

void BMainWindow::onDW5Changed(bool b){
   ui.actionVerse_collectien_toolbox->setChecked(b);
};

void BMainWindow::onWindowsNext(){
   mdiArea->activateNextSubWindow();
};

void BMainWindow::onWindowsTile(){
   doTile = true;
   mdiArea->tileSubWindows();
   activeBibleMaximized = false;
};

void BMainWindow::onWindowsCascade(){
   doTile = false;
   mdiArea->cascadeSubWindows();
   activeBibleMaximized = false;
};

void BMainWindow::onWindowsCloseAll(){
   mdiArea->closeAllSubWindows();
};

void BMainWindow::onWindowsCloseOthers(){
   QMdiSubWindow *cw = mdiArea->activeSubWindow();
   while(mdiArea->subWindowList().size()>1){
      mdiArea->activateNextSubWindow();
      if (mdiArea->activeSubWindow()!=cw) mdiArea->closeActiveSubWindow();
   }
   tileOrCascade();
};

void BMainWindow::onWindowsCloseActive(){
   mdiArea->closeActiveSubWindow();
};

void BMainWindow::onWindowsCrossBGBible(){
   QString p = preferences()->CrossBgBiblePath();
   QString d = QFileInfo(p).absolutePath();
   myProcess->setWorkingDirectory(d);
#ifdef Q_WS_MAC
   p = p + QDir::separator() + "Contents/MacOS/CrossBgBible";
#else
#endif
   myProcess->start(p);
};

void BMainWindow::onWindowsVersion43(){
#ifdef Q_WS_WIN
   QString p = QFileInfo(biblePath()+"CDBible40.exe").absoluteFilePath();
   if (QFileInfo(p).exists()){
      QString d = QFileInfo(p).absolutePath();
      showMessage(tr("While your are working with version 4.3, the word lists in %1 of the same Bibles wil not be visible.").arg(progVersion));
      myProcess->setWorkingDirectory(d);
      myProcess->setParent(0);
      myProcess->start(p);
   }
   else downloadV43();
#else
   showMessage(tr("This feature is available only for Windows."));
#endif
};

void BMainWindow::onWindowsPreferences(){
   preferences()->exec();
};

void BMainWindow::onChangeBaseColor(const QColor &c){
   QPalette::ColorRole r = QPalette::Base;
   preferedColor()->setColorTo(ui.comboBox, r, c);
   preferedColor()->setColorTo(ui.comboBox_2, r, c);
   preferedColor()->setColorTo(ui.comboBox_3, r, c);
   preferedColor()->setColorTo(ui.comboBox_4, r, c);
   preferedColor()->setColorTo(ui.listView, r, c);
   preferedColor()->setColorTo(ui.listView_2, r, c);
   QList<QMdiSubWindow *> swl = mdiArea->subWindowList();
   for(int i=0; i<swl.size(); i++) preferedColor()->setColorTo(swl.at(i)->widget(), r, c);
};

void BMainWindow::onChangeTextColor(const QColor &c){
   QPalette::ColorRole r = QPalette::Text;
   preferedColor()->setColorTo(ui.comboBox, r, c);
   preferedColor()->setColorTo(ui.comboBox_2, r, c);
   preferedColor()->setColorTo(ui.comboBox_3, r, c);
   preferedColor()->setColorTo(ui.comboBox_4, r, c);
   preferedColor()->setColorTo(ui.listView, r, c);
   preferedColor()->setColorTo(ui.listView_2, r, c);
   emit refreshText();
};

void BMainWindow::onHelpContent(){
   myProcess->browse(progDir() + "htdocs/help/help.html");
};

void BMainWindow::onHelpReadme(){
   myProcess->browse(progDir() + "htdocs/readme-"+interfaceLanguage()+".html");
};

void BMainWindow::onHelpAboutProgram(){
   showMessage(tr("Read the Bible 5 - v%1<br>Copyright (C) 2008  Vanyo Georgiev<br>&lt;<A HREF=mailto:%3>%3</A>&gt;<br><A HREF=%2>%2</A><br><br>This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.<br><br>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.<br><br>You should have received a copy of the GNU General Public License along with this program; if not, write to the<br><br>Free Software Foundation, Inc.,<br>51 Franklin Street, Fifth Floor,<BR>Boston, MA  02110-1301, USA.").
       arg(progVersion,progURL+interfaceLanguage(),progEmail) );
};

void BMainWindow::onHelpAboutBibleVersion(){
   BibleWindow *ab = activeBible();
   if (!ab) return;
   ab->about(myProcess);
};

void BMainWindow::onHelpWebSite(){
   myProcess->browse(progURL+interfaceLanguage());
};

void BMainWindow::onHelpCheckForUpdate(){
   webUpdater->checkForUpdates("/bible/v5/", progVersion);
};

void BMainWindow::onMaxItemsChanged(int i){
   i--;
   ui.comboBox_2->setMaxVisibleItems(i);
   ui.comboBox_3->setMaxVisibleItems(i);
   ui.comboBox_4->setMaxVisibleItems(i);
};

void BMainWindow::onVerseClick(BibleWindow *ab, int i){
   goByIndex(ab, i);
};

// Намира индекса на текста, изписан в QComboBox
// и прави този индекс текущ.
void setIndexByText(QComboBox *cb){
   QString s = cb->currentText();
   int i = cb->findText(s,Qt::MatchFixedString);
   if (i>-1) cb->setCurrentIndex(i);
};

// Установява книга,глава,стих на активния прозорец
// според изписаното в трите comboBox-а.
// Ако si==true се установяват индексите да съответстват на изписаното.
BibleWindow * BMainWindow::setActiveBibleReference(bool si){
   BibleWindow *ab = activeBible();
   if (!ab) return 0;
   if (si) {
     setIndexByText(ui.comboBox_2);
     setIndexByText(ui.comboBox_3);
     setIndexByText(ui.comboBox_4);
   }
   QString s = ui.comboBox_2->currentText();
   int i = ui.comboBox_2->findText(s,Qt::MatchFixedString);
   if (i<0) return ab;
   ab->setReference(
      i,
      ui.comboBox_3->currentText().toInt()-1,
      ui.comboBox_4->currentText().toInt()-1
   );
   return ab;
};

// Изпращане на сигнал, че е променен активния стих
void BMainWindow::emitIndexChanged(BibleWindow *ab){
   int gi = ab->localToGlobalIndex(); // Глобален индекс на активния сих от активната библия
    if (gi!=globalVerseIndex()){ // Записване в историята, ако е различен от текущия
        history->push(gi);
    }
   setGlobalVerseIndex(gi);     // Новия става текущ
   emit globalIndexChanged(ab); // Изпращане на сигнал
   ab->setReadPos();
   updateNavButtons();
};

// Създаване на прозорец с Библия от директория bv
BibleWindow *BMainWindow::openBible(const QString &bv){
   static int n = 0; // уникален номер на поредния създаден прозорец с Библия
   BibleWindow *bw = new BibleWindow(bv);
   if (!bw->longTitles.size()) return bw;
   connect(this, SIGNAL(globalIndexChanged(BibleWindow*)), bw, SLOT(onGlobalIndexChanged(BibleWindow*)));
   connect(this, SIGNAL(scrollToActiveVerse()), bw, SLOT(scrollToActiveVerse()));
   connect(this, SIGNAL(refreshText()), bw, SLOT(refreshText()));
   connect(bw, SIGNAL(closing(BibleWindow*)), this, SLOT(onBibleWindowClosing(BibleWindow*)));
   connect(bw, SIGNAL(destroyed(QObject*)), this, SLOT(onBibleWindowDestroyed(QObject*)));
   connect(bw, SIGNAL(anchorClicked(QUrl)), this, SLOT(onBibleAnchorClicked(QUrl)));
   connect(preferences(), SIGNAL(optionChanged()), bw, SLOT(refreshText()));
   connect(preferedColor(), SIGNAL(toChangeOtherTextColor()), bw, SLOT(refreshText()));
   bw->setObjectName( "b"+QString::number(n) ); n++;
   mdiArea->addSubWindow(bw);
   bibleWindow.insert(bw->objectName(), bw);
   bw->globalIndexToLocal();
   bw->show();
   bw->resize(600,400);
   bw->displayText();
   return bw;
};

bool BMainWindow::isOpened(const QString &bv){
   QList<QMdiSubWindow *> swl = mdiArea->subWindowList();
   for(int i=0; i<swl.size(); i++){
      QString objn = swl.at(i)->widget()->objectName();
      BibleWindow *bw = bibleWindow.value( objn );
      if ( bw->bibleVersion() == bv) return true;
   }
   return false;
};

int BMainWindow::windowCount(const QString &bv){
   int r = 0;
   QList<QMdiSubWindow *> swl = mdiArea->subWindowList();
   for(int i=0; i<swl.size(); i++){
      QString objn = swl.at(i)->widget()->objectName();
      BibleWindow *bw = bibleWindow.value( objn );
      if ( bw->bibleVersion() == bv) r++;
   }
   return r;
};

void BMainWindow::writeSettings(){
   QSettings s("VanyoG", "CD Bible 5");
   s.setValue("biblePath",biblePath());
   s.setValue("version",progVersion);
   s.setValue("windowState", saveState() );
   s.setValue("windowGeometry", saveGeometry() );
   s.setValue("docWidgetGeometry", ui.dockWidget->saveGeometry() );
   s.setValue("docSplitterState", ui.splitter->saveState() );
   s.setValue("globalVerseIndex",globalVerseIndex());
   s.setValue("tileOrCascade",doTile);
   preferences()->writeSettings(&s);
   QList<QMdiSubWindow *> swl = mdiArea->subWindowList(QMdiArea::ActivationHistoryOrder);
   QStringList bl;
   for(int i=0; i<swl.size(); i++){
      BibleWindow *bw = bibleWindow.value( swl.at(i)->widget()->objectName() );
      if (bw){
         bl << bw->bibleVersion();
         bw->writeSettings();
      }
   }
   s.setValue("openedBibles",bl);
   QMdiSubWindow *aw = mdiArea->activeSubWindow();
   if (aw) s.setValue("activeMaximized", aw->windowState().testFlag(Qt::WindowMaximized) );
   s.setValue("updateFile",webUpdater->downloadedFile());
   bl = getReadPositions();
   if (bl.size()) s.setValue("readPositions",bl);
   s.setValue("lastVersion",lastVersion);
};

void BMainWindow::readSettings(){
   do_Not_Exec = false;
   
   setDefaultCss(fileContent(styleFile));

   QSettings s("VanyoG", "CD Bible 5");

   QString st; // Сринг, който ще се използва за проверка на различни запазени стрингови стойности
   
   if (progVersion!=s.value("version").toString()) newVersion();
   else{
     st = s.value("biblePath").toString();
     // Задава стойността, която се чете с biblePath() - път до директорията с библии
     if (st.size()) setBiblePath(st);
   }

   // Стойността "downloadSite" се въвежда ръчно, ако е необходимо сайта за изтeгляне да е различен от подразбиращия се
   st = s.value("downloadSite").toString();
   if (st.size()) setDownloadSite(st);
   webUpdater->setHost( downloadSite() );

   // Състояние на главния прозорец
   // Оказва се, че това възстановява и размерите и състоянията на панелите и менютата.
   QByteArray ws = s.value("windowState").toByteArray();
   if (!ws.size()) return;
   else restoreState( ws );

   // Позиция на десния край на панела за търсене
//   ui.splitter->restoreState( s.value("docSplitterState").toByteArray() );
   
//   ui.action_Searching_toolbox->setChecked( !ui.dockWidget->isHidden() );

   // Глобален номер на текущия стих
   setGlobalVerseIndex(s.value("globalVerseIndex",globalVerseIndex()).toInt());
   
   // История на последно отваряните стихове
   history->push(globalVerseIndex());
   
   doTile=s.value("tileOrCascade").toBool();
   
   // Зареждане на текущите настройки в прозореца за настройки
   preferences()->readSettings(&s);
   
   // Отваряне на последно отваряните библии
   QStringList bl = s.value("openedBibles").toStringList();
   activeBibleMaximized = s.value("activeMaximized").toBool();
   for(int i=0; i<bl.size(); i++){
      BibleWindow *bw = openBible(bl.at(i));
      if (activeBibleMaximized)
         bw->setWindowState(Qt::WindowMaximized | Qt::WindowActive);
   }
   bl = s.value("readPositions").toStringList();
   setReadPositions(bl);
   
   // Размери и позиция на главния прозорец
   restoreGeometry( s.value("windowGeometry").toByteArray() );

   // Ако е запазено име на файл, съдържащ нова версие се стартира програмата за актуализиране
   do_Not_Exec = webUpdater->updateFromFile( s.value("updateFile").toString() );
   
   // Посведно отворения превод на Библията, използва се за "Отвори отново"
   lastVersion = s.value("lastVersion").toString();
   
   ui.action_Searching_toolbox->setChecked(ui.dockWidget_5->isVisible());
   ui.actionVerse_collectien_toolbox->setChecked(ui.dockWidget->isVisible());
};

void BMainWindow::createBActions(){
   styleFile = progDir() + "data/"+styleFile;
   QString fn = progDir()+"data/bibles/list-u.txt";
   setBiblePath(progDir()+"data/bibles/");
   QString fc = fileContent(fn,"UTF-8"), lng = "";
   QStringList bs = fc.split("\n");
   bool said = false;
   QAction *ba = ui.actionOpen_again;
   for(int i=0; i<bs.size(); i++)
    if ((bs.at(i).size())&&(bs.at(i).at(0).unicode()!=QChar('#').unicode()))
   {
      QStringList b = bs.at(i).trimmed().split("=");
      if (b.size()==4){
         if (i==0) lng = b.at(3);
         setVersionAttributes(b.at(0), b.at(1), b.at(2), b.at(3) );
         // добавяне на сепаратор между библиите на различни езици
         if (lng!=b.at(3)){
            QAction *ac = new QAction(this);
            ac->setSeparator(true);
            ui.menu_Bible_1->insertAction(ba,ac);
            lng=b.at(3);
         }
         // добавяне на поредната Библия
         QAction *ac = new QAction(b.at(1),this);
         ac->setCheckable(true);
         ac->setObjectName(b.at(0));
         bAction.insert(b.at(0), ac);
         ui.menu_Bible_1->insertAction(ba,ac);
      }
      else {
         if (!said) showMessage( tr("File %1 is corrupt at line %2.").arg(fn).arg(i) );
         said = true;
      }
   }
   // добавяне на последния сепаратор преди Download
   QAction *ac = new QAction(this);
   ac->setSeparator(true);
   ui.menu_Bible_1->insertAction(ba,ac);
};

// Пълни падащите списъци за избиране на номера (на глави и стихове)
void BMainWindow::setNumberComboBox(QComboBox *cb, int max, int curr){
   cb->clear();
   if (max<1) max=1;
   for(int i=1; i<=max; i++) cb->addItem(QString::number(i));
   if (curr>max) curr = max;
   cb->setCurrentIndex(curr-1);
};

 BibleWindow *BMainWindow::activeBible(){
    BibleWindow *bw = 0;
    // Указател към прозореца, който последно е бил активен
    static QMdiSubWindow *oaw = 0;
    QMdiSubWindow *aw = mdiArea->activeSubWindow();
    // Понякога в mdiArea няма активан прозорец затова трябва да се
    // активира отново прозореца, който последно е бил активен
    if (!aw){
       QList<QMdiSubWindow*> swl = mdiArea->subWindowList();
       int i = swl.indexOf(oaw);
       if (i>-1){
          mdiArea->setActiveSubWindow(oaw);
          aw = mdiArea->activeSubWindow();
       }
       else return bw;
    }
    oaw = aw;
    // Връща се указател към прозореца на активната Библия
    bw = bibleWindow.value( aw->widget()->objectName() );
    return bw;
 };

void BMainWindow::tileOrCascade(){
   if (activeBibleMaximized){
      QMdiSubWindow *aw = mdiArea->activeSubWindow();
      if (aw){
         aw->setWindowState(Qt::WindowMaximized | Qt::WindowActive);
         aw->show();
      }
      return;
   }
//   showMessage("T or C");
   if (doTile){
       mdiArea->tileSubWindows();
   }
   else
       mdiArea->cascadeSubWindows();
   emit scrollToActiveVerse();
};

// Прави активни бутоните за навигация "Напред" и "Назад"
void BMainWindow::updateNavButtons(){
    bool p3 = history->index()>0;
    bool p4 = history->index() < history->count()-1;
    ui.pushButton_3->setEnabled(p3);
 //   if(p3) ui.pushButton_3->setFocus();
    ui.pushButton_4->setEnabled(p4);
 //   if(p4) ui.pushButton_4->setFocus();
}

// Ъпдейтва контролите при смяна на текущия стих
void BMainWindow::updateControls(BibleWindow *ab){
   int b = ab->book()-1;
   if ( b != ui.comboBox_2->currentIndex() ){
      ui.comboBox_2->setCurrentIndex(b);
      setNumberComboBox(ui.comboBox_3, ab->chapterCount(), ab->chapter() );
      setNumberComboBox(ui.comboBox_4, ab->verseCount(), ab->verse() );
      emitIndexChanged(ab);
   }
   else {
      int c = ab->chapter()-1;
      if (c != ui.comboBox_3->currentIndex() ){
         ui.comboBox_3->setCurrentIndex(c);
         setNumberComboBox(ui.comboBox_4, ab->verseCount(), ab->verse() );
         emitIndexChanged(ab);
      }
      else {
         int v = ab->verse()-1;
         ui.comboBox_4->setCurrentIndex(v);
         emitIndexChanged(ab);
      }
   }
};

// Установява флагът activeBibleMaximized, който означава дали активната библия е максимицирана
void BMainWindow::checkMaximization(){
   QMdiSubWindow *aw = mdiArea->activeSubWindow();
   if (aw) activeBibleMaximized = aw->windowState().testFlag(Qt::WindowMaximized);
};

// Отиване до номер на стих i от текущата библия
void BMainWindow::goByIndex(BibleWindow *ab, int i){
    ab->setVerseByIndex(i);
    updateControls(ab);
};

void BMainWindow::goByGlobalIndex(int i){
  if (i<0) return;
  BibleWindow *bw = activeBible();
  if (!bw) return;
  setGlobalVerseIndex(i);
  bw->globalIndexToLocal();
  updateControls(bw);
};

void BMainWindow::setSynchronization(BibleWindow *ab){
   QList<QMdiSubWindow *> sw = mdiArea->subWindowList();
   for(int i=0; i<sw.size(); i++){
      BibleWindow *bw = bibleWindow.value( sw.at(i)->widget()->objectName() );
      if (bw){
        if (ab->bibleVersion()==bw->bibleVersion()) bw->synchronize = (bw==ab);
      }
   }
};

void BMainWindow::downloadV43(){
#ifdef Q_WS_WIN
   if (yesNo(tr("Version 4.3 is not Installed. Do you want to download it?"))==NO) return;
   if ( ((QSysInfo::windowsVersion()==QSysInfo::WV_VISTA) || (QSysInfo::windowsVersion()==QSysInfo::WV_WINDOWS7))
     && (qApp->applicationDirPath()=="C:/Program Files (x86)/VanyoG/Bible5")
   ){
     QString ms = tr("When download on Windows Vista or Windows 7 you have to run this progrm as Administrator."
                           "(Right click on C:\\Program files (x86)\\VanyoG\\Bible5\\Bible.exe and click on \"Run as Administrator\"). Do you run it as Administrator?"
                        ); 
     if (yesNo(ms)==NO) return;
   }
   if (!fileDownloader) fileDownloader = new FileDownloader(this, ui.progressBar, ui.pushButton);
   QString u = "http://" + downloadSite() + "/bible/v5/CDBible40.zip";
   QString p = QFileInfo(biblePath()+"CDBible40.zip").absoluteFilePath();
   fileDownloader->downloadAndUnzip(u, p, tr("Version 4.3"));
#endif
};

void BMainWindow::newVersion(){
#ifdef Q_WS_WIN
  QString d = "C:\\Program Files\\VanyoG\\Bible\\";
  if (QFileInfo(d+"CDBible40.exe").exists() && 
     (yesNo(tr("Version 4.3 is found in %1. Do you want to use bibles from this version to save disk space.").arg(d))==YES)
  ){
    setBiblePath(d);
  }
  else
#endif
  showMessage(tr("Congratulations. You started the new version %1. Please, write to<br><a href=\"mailto:%2\">%2</a><br>for any questions or bug reporting.").arg(progVersion,progEmail));
};

void BMainWindow::downloadActiveBible(){
   BibleWindow *ab = activeBible();
   if (!ab){
      showMessage(tr("Open the Bible Translation you like to download and try again."));
      return;
   }
   ab->downloadBible(ab->bibleVersion());
};

QStringList lang_list;
QString int_lang = QLocale::system().name().left(2);

void setIntegfaceLanguage(){
   lang_list << "" << "bg" << "mk" <<"en";
   QSettings s("VanyoG", "CD Bible 5");
   int i = s.value("language").toInt();
   if (i>0) int_lang = lang_list.at(i);
};

QString interfaceLanguage(){
   return int_lang;
};
