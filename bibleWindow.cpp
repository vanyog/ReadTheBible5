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

#include "bibleWindow.h"
#include "showMessage.h"
#include "myDecode.h"
#include "myFileRoutines.h"
#include "language.h"
#include "fileDownloader.h"
#include "preferences.h"
#include "process.h"
#include "concordance.h"

#include <QHash>
#include <QFileInfo>
#include <QApplication>
//#include <QTextCodec>
#include <QProgressBar>
#include <QPushButton>
#include <QTextBlock>
#include <QTextDocumentFragment>
#include <QTextStream>
#include <QDir>

QProgressBar  *downloadProgressBar = 0;
QPushButton *dounloadCancelButton = 0;
QString bible_Path = "not set";
QHash<QString, int> read_pos;
//QHash<QString, ConcordanceModel*> concordanceHash;

bool caseInsensitiveLessThan(const QString &s1, const QString &s2){
  return s1.toLower() < s2.toLower();
}

//------------------VerseCorrection---------------------

VerseCorrection::VerseCorrection(const QStringList &cl){
   bk = cl.at(0).toInt();
   ch0 = cl.at(1).toInt();
   vr0 = cl.at(2).toInt();
   ch1 = cl.at(3).toInt();
   vr1 = cl.at(4).toInt();
};

//-------------------BibleWindow------------------

BibleWindow::BibleWindow(const QString &bv,  QWidget *parent)
   :QTextBrowser(parent)
{
   setAttribute(Qt::WA_DeleteOnClose);
   bible_Version = bv;
   zipFile = bv+".zip";
   setOpenLinks(false);
#ifdef Q_WS_WIN
   dir = bible_Path+bv+"\\";
#else
   dir = bible_Path+bv+"/";
#endif
   zipFile = dir+zipFile;
#ifdef Q_WS_MAC
   QString sf = "style.css";
#else
   QString sf = "style-w.css";
#endif
   if (QFileInfo::exists(dir+sf)) css = fileContent(dir+sf);
   else css = defaultCss();
   process = 0;
   setWindowTitle( versionCaption(bv) );
   bkl=0; chl=0; vrl=0;  fileDownloader=0;
   synchronize = true;
   wordsChanged = false;
   if (!readTitles()){
      downloadBible(bv);
      return;
   };
   bible_Structure = readStructure(dir+"BibleStructure.csv");
   readCorrection();
   concordance_ = new ConcordanceModel(dir, versionCodec(bv), this);
/*   concordance_ = concordanceHash.value(bible_Version);
   if (!concordance_){
     concordance_ = new ConcordanceModel(dir, versionCodec(bv), this);
     concordanceHash[bible_Version] = concordance_;
   }*/
};

bool BibleWindow::readTitles(){
    QString fn  = dir+"BibleTitles.txt";
   QString fnu = dir+"BibleTitles-u.txt";
   if (!QFileInfo::exists(fn)) return false;
   QString fc="";
   if (QFileInfo::exists(fnu)) fc = fileContent(fnu, "UTF-8" );
   else fc = fileContent(fn, versionCodec(bibleVersion()) );
   if (!fc.size()) return false;
   QStringList fl = fc.split("\n");
   if (!fl.size()) return false;
   QStringList l0 = fl.at(0).split(" ");
   book_count = l0.at(0).toInt();
   if (!book_count) return false;
   for(int i=1; i<=book_count; i++){
      int j = l0.at(i).toInt();
      global_book.insert( i, j );
      local_book.insert( j, i );
      longTitles << fl.at(i);
      shortTitles << fl.at(i+book_count);
      QString a = fl.at(i+2*book_count);
      abrevs << a;
      book_index[a] = i;
   }
   return true;
};

void BibleWindow::readCorrection(){
  QString fn = dir+"_Diff_.txt";
  if ( !QFileInfo::exists(fn) ) return;
  QString fc = fileContent(fn);
  QStringList fl = fc.split("\n");
  for(int i=0; i<fl.size(); i++){
     QStringList cl = fl.at(i).trimmed().split(" ");
     if (cl.size()==5){
        VerseCorrection *vcLG = new VerseCorrection(cl), *vcGL = new VerseCorrection(cl);
        vcGL->ch0 = vcGL->ch0 + vcGL->ch1;
        vcGL->vr0 = vcGL->vr0 + vcGL->vr1;
        correctionLG.append(vcLG);
        correctionGL.append(vcGL);
     }
  }
};

// Стартира изтеглянето на библията
void BibleWindow::downloadBible(const QString &bv){
   Q_UNUSED(bv);
   if (yesNo( tr("Would you like to download Bible %1?").arg(windowTitle()) )!=YES) return;
   if (!fileDownloader){
      fileDownloader = new FileDownloader( this, downloadProgressBar, dounloadCancelButton);
      connect(fileDownloader, SIGNAL(done(bool)), this, SLOT(onBDownloadDone(bool)));
   }
   QString of = "https://"+downloadSite()+"/bible/v5/"+QFileInfo(zipFile).fileName();
//   fileDownloader->downloadFile(of,zipFile);
   fileDownloader->downloadAndUnzip(of,zipFile,windowTitle());
};

void BibleWindow::writeSettings(){
   concordance()->writeSettings();
};

void BibleWindow::readSettings(){

};

void BibleWindow::createLinksBin(){
   QString fln = dir+"Links.txt";
   QFile lf(fln);
   if (!lf.exists()) {
      showMessage(tr("File do not exist\n%1").arg(fln));
      return;
   }
   QString idfn = dir+"Links.bin"; // Двоични файлове, в които се записват препратките
   QString ipfn = dir+"LinksP.bin";
   QFile idf(idfn);
   QFile ipf(ipfn);
   if (!idf.open(QFile::WriteOnly)){
      showMessage(tr("Can't open file for write\n%1").arg(idfn));
      return;
   }
   if (!ipf.open(QFile::WriteOnly)){
      showMessage(tr("Can't open file for write\n%1").arg(ipfn));
      return;
   }
   QDataStream ds(&idf);
   QDataStream ps(&ipf);
   ds.setByteOrder(QDataStream::LittleEndian);
   ps.setByteOrder(QDataStream::LittleEndian);
   int vi0 = 1;   // Локален индекс на последния стих, за който са записани препратки
   qint32 m1 = -1; // Константа -1, която се записва вместо указател за стиховете, от които няма препратки
   QString flc = fileContent(fln); // Чете се файла с описание на препратките
   QStringList lns = flc.split("\n"); // Разделя се на списък от редове
   for (int i=0; i<lns.size(); i++){ // Обработва се всеки ред
      QString ln = lns.at(i).trimmed();   // Това е i-ия ред
      if (!ln.size()) continue;
      QStringList lks = ln.split(QChar(9)); // Реда се разделя на две части от табулатор
      if (lks.size()<2){                      // Ако няма табулатор реда се прескача
        showMessage( tr("There is no Tab symbol in this line:\n%1").arg(ln) );
        continue;
      }
      QString ln1 = lks.at(0).trimmed();    // Частта пред табулатора означава за кой стих се отнасят препратките
      int vi = verseIndex(ln1);     // Локален индекс на стиха, за който се отнасят препратките
      for(int j=vi0+1; j<vi; j++){ // Записва -1 вместо указатели, за стиховете до тук, от които няма препратки
         ps << m1;
      }
      vi0 = vi;
      qint32 p = idf.pos()/2; ps << p;   // Записва указател в LinksP.bin
      if (vi*4!=ipf.pos()){ showMessage(ln); break; }
      QStringList ls = lks.at(1).trimmed().split(","); // Останалата част от реда съдържа препратки, разделени със запетая
      QList<quint16> ll;                 // Списък с локални индекси на стихове, към които има препратки от стих vi
      for (int j=0; j<ls.size(); j++){   // Обработва се всяка препратка.
        QString lk = ls.at(j).trimmed(); // Поредната препратка
        QStringList vs = lk.split("-");  // Препратката се разделя на две части от тире.
        QString vs1 = vs.at(0).trimmed();// Първата част
        if (!vs1.size()){                // Ако първата част е празна
          showMessage(lks.at(1));
          continue;
        }
        int li = verseIndex(vs1); // Локален индекст на стиха, към който сочи първата част на препратката
        if (li<0) showMessage(ln);
        ll << li;
        if (vs.size()>1){ // Ако е имало тире, се добавят и всички стихове до посочения след тирето
           bool ok;
           int v2 = vs.at(1).trimmed().toInt(&ok); // Номер на стиха след тирето
           if (!ok) showMessage( tr("Can't recognize verse number in reference %1").arg(lk) );
           else for(int v = li+1; v <= v2; v++) ll << v;
        }
      }
      quint16 sz = ll.size(); // Записва препратките в Links.bin
      ds << sz;
      for(int j=0; j<sz; j++) ds << ll.at(j);
   }
   showMessage(reference(vi0));
   for(int i=vi0+1; i<verseTotalCount()+1; i++) ps << m1; // Допълва файла с указатели до последния стих на Библията
   idf.close(); ipf.close();
   showMessage(tr("Link file was created."));
};

// Импортиране на Библия от .txt файл fn.
// Новите файлове се записват в нова директория с добавен "-" в края на името
void BibleWindow::import(const QString &fn){
   downloadProgressBar->setVisible(true);    // Показване на лентата за напредването на работата
   QStringList fc = fileContent(fn).split("\n"); // Прочитане текста на Библията от избрания файл
   downloadProgressBar->setMaximum(fc.count());
   int i = 0;                                    // Номер на обработвания ред текст
   QStringList tx;                               // Списът с прочетените стихове
   QString chs = languageObject()->characters(language()); // Буквите на езика
   for(int b=1; b<=bookCount(); b++){                      // Цикъл за четене на книгите
     // Чете се заглавието на книгата bt
     QString bt = fc.at(i); i++;
     // Ако редът текст е празен се търси следващ непразен ред.
     while (!bt.trimmed().size()){ bt = fc.at(i); i++; }
     // Заглавията на книгите са на два реда
     bt = bt + " " + fc.at(i); i++; // Добавя се втория ред
     if (bt.trimmed()!=longTitles.at(b-1)){ // Ако заглавието не е коректно обработката се прекратява
       showMessage(tr("Incorrect Book title.<br>%1<br>Must be %2").arg(bt,longTitles.at(b-1)));
       return;
     }
     for(int c=1; c<=chapterCount(b); c++){ // Цикъл за четене на главите ат поредната книга с номер b
        for(int v=1; v<=verseCount(b,c); v++){ // Цикъл за четене на стиховете от поредната глава с номер c
          downloadProgressBar->setValue(i);

          QString r = reference(verseIndex(b,c,v));

          QString vn = QString::number(v)+" ";  // Номер, с който започва реда с поредния стих
          if (v==1) vn = QString::number(c)+" ";// Ако стихът е първи той започва с номара на главата

          QString vt = ""; // Следващ ред текст
          // Взема се следващия непразен ред текст.
          do { vt = fc.at(i); i++; } while (!vt.trimmed().size());

          // Проверява се дали редът не е подзаглавие
          if (vt.startsWith("<h")){
            // Взема се следващия непразен ред текст.
            do { vt = fc.at(i); i++; } while (!vt.trimmed().size());
          }
          
          // Проверява се дали има паралелен пасаж.
          if (vt.at(0)==QChar('{')){
            QString pp = vt;
            vt = fc.at(i); i++;
          }
          
          bool toAdd = true;
          if (chs.indexOf(vt.at(0))>-1){ // Ако редът започва с буква, то той е част от предишния стих
             toAdd = false;
          }
          else {   
             if (!vt.startsWith(vn)){ // Ако номерът на стиха е некоректен обработката се прекратява
               showMessage(tr("%1<br>Incorrect verse number in line<br>%2.<br>Must be %3").arg(r,vt,vn)
               );
               return;
             }
             vt = vt.right(vt.size()-vn.size()); // Премахва се номера от пред текста на стиха
          }

          // Проверява се дали има бележка под линия към стиха
          int p = vt.indexOf("*");
          if (p>-1) { // Изпълнява се ако има бележка под линия
            QString ft = fc.at(i); i++; // Чете се следващия ред, който е бележка под линия
            ft = ft.right(ft.size()-1); // Премахва се първия символ *
            vt.replace("*"," {"+ft+"}"); // Вмъква се бележката под линия
          }
          
          // Проверява се дали има препратки към стиха
          int j=1; // Номер на поредната група препратки
          QString ln = "(" + QString::number(j) + ") ";
          if (fc.at(i).startsWith(ln)){
            int p1 = 0;
            QString lt = fc.at(i); i++; // Чете се следващия ред, съдържащ препратки
            while (lt.startsWith(ln)){
              lt = lt.right(lt.size()-ln.size());  // Изтрива се номера на групата препратки
              p1 = vt.indexOf(QString::number(j),p1); // Позиция на мястото на групата препратки
              j++; ln = "(" + QString::number(j) + ") "; // Номер на следващата група препратки
              p = lt.indexOf(ln);       // Позиция на номера на следващата група препратки
              QString lt1 = lt.left(p); // Отделя се поредната група препратки
              lt = lt.right(lt.size()-lt1.size()); // Премахва се отелената група препратки
              vt.insert(p1," {"+lt1+"}"); // Вмъква се групата препратки в текста на стиха
              p1 += lt1.size()+3;
            }
          }
          
          // Добавяне на прочетения стих в списъка
          if (toAdd) tx << vt;
          else {
             int k = tx.size()-1;
             tx[k] = tx.last()+"<br>"+vt;
             v--;
             showMessage(r+"\n"+tx.at(k));
          }
          
        } // Край на цикъла за четене на стиховете от поредната глава
     } // Край на цикъла за четене на главите от поредната книга
   } // Край на цикъла за четене на книгите
   downloadProgressBar->reset();
   downloadProgressBar->setMaximum(tx.size());
      
   // Съставяне на списък от думите в Библията
   QStringList wl; // Списък на думите в Библията
   for(int i=0; i<tx.size(); i++){ // Цикъл за обработка на стиховете
      downloadProgressBar->setValue(i);
      QString w;          // Поредна дума
      bool inWord = false;// Дали е открито начало на дума
      int j1 = 0, j2 = 0; // Начало и край на поредната дума
      for(int j=0; j<tx.at(i).size(); j++){ // Цикъл за обработка на поредния стих
         if (chs.indexOf(tx.at(i).at(j))>-1){ // Ако поредния символ е буква
            j2 = j; 
            if (!inWord){ j1 = j; inWord = true; }
         }
         else { // Ако поредния символ не е буква
            if (inWord){
              w = tx.at(i).mid(j1,j2-j1+1); // Отделя се поредната дума
//              showMessage(w);
              if (!wl.contains(w)) wl << w; // Думата са добавя в списъка
              inWord = false;
            }
         }
      }
      if (inWord){ // Ако стихът завършва с буква, се добавя и последната дума от стиха
         w = tx.at(i).mid(j1);
         if (!wl.contains(w)) wl << w;
      } 
   }
   std::sort(wl.begin(), wl.end(), caseInsensitiveLessThan);
   
   // Изтриване на думите с главна буква, ако се срещат и с малка буква
   for(int i=wl.count()-1; i>0; i--) if (wl.at(i).toLower()==wl.at(i-1).toLower()){
      if (wl.at(i).at(0).isUpper()) wl.removeAt(i);
      if (wl.at(i-1).at(0).isUpper()) wl.removeAt(i-1);
   }
   
   // Ако новата директория не съществува, то тя се създава
   QString nn = bible_Version + "-";
   QString np = bible_Path;
   if (!QFileInfo::exists(np+nn)) QDir(np).mkdir(nn);
   nn = np+nn;

   // Запис на текста
   QFile ct(nn+"/CompactText.bin");
   QFile cp(nn+"/CompactPoint.bin");
   bool ok = ct.open(QIODevice::WriteOnly);
   ok = cp.open(QIODevice::WriteOnly);
   QDataStream ts(&ct);
   QDataStream ps(&cp);
   ts.setByteOrder(QDataStream::LittleEndian);
   ps.setByteOrder(QDataStream::LittleEndian);
//   QTextCodec *tc = QTextCodec::codecForName(versionCodec(bibleVersion()));
 //  QTextCodec::setCodecForCStrings(tc);
   qint32 p = 0;
   for(int i=0; i<tx.size(); i++){
       ps << p;
       qint16 c = tx.at(i).size();
       ts << c; //countedMessage(tx.at(i+" "+QString::number(p));
       QByteArray ba = tx.at(i).toUtf8();
       ts.writeRawData(ba.data(),c);
//       ts << ba;
       p = ct.pos();
   }
   ct.close();
   cp.close();

   // Запис на думите
#ifdef Q_WS_WIN
   saveToFile(nn+"/WordList.txt",wl.join("\n"));
#else
   saveToFile(nn+"/WordList.txt",wl.join("\r\n"));
#endif
   cp.setFileName(nn+"/WordPoint.bin");
   ok = cp.open(QIODevice::WriteOnly);
   ps.setDevice(&cp);
   p = 0;
   for(int i=0; i<wl.size(); i++){
       ps << p;
       p += wl.at(i).size() + 2;
   }
   cp.close();
   downloadProgressBar->reset();

   // Създаване на конкорданс
   QHash<QString, QList<int>* > cc;
   for(int i=0; i<tx.size(); i++){
       downloadProgressBar->setValue(i);
       // Това повтаря цикъла за намиране на думите. Може да се помисли в бъдеще двата цикъла да се обединят.
       QString w;          // Поредна дума
       bool inWord = false;// Дали е открито начало на дума
       int j1 = 0, j2 = 0; // Начало и край на поредната дума
       for(int j=0; j<tx.at(i).size(); j++){ // Цикъл за обработка на поредния стих
          if (chs.indexOf(tx.at(i).at(j))>-1){ // Ако поредния символ е буква
             j2 = j;
             if (!inWord){ j1 = j; inWord = true; }
          }
          else { // Ако поредния символ не е буква
             if (inWord){
               w = tx.at(i).mid(j1,j2-j1+1).toLower(); // Отделя се поредната дума
               QList<int> *l;
               if (!cc.contains(w)){
                   l = new QList<int>;
                   cc[w] = l;
               }
               else l = cc[w];
               int s = l->size();
               if ( !s || (l->at(s-1)!=i+1) ) l->append(i+1);
               inWord = false;
             }
          }
       }
       if (inWord){ // Ако стихът завършва с буква, се добавя и последната дума от стиха
          w = tx.at(i).mid(j1).toLower();
          QList<int> *l;
          if (!cc.contains(w)){
              l = new QList<int>;
              cc[w] = l;
          }
          else l = cc[w];
          int s = l->size();
          if ( !s || (l->at(s-1)!=i+1) ) l->append(i+1);
       }
   }
   // Записване на конкорданса
   ct.setFileName(nn+"/Conc.bin");
   cp.setFileName(nn+"/ConcP.bin");
   ok = ct.open(QIODevice::WriteOnly);
   ok = cp.open(QIODevice::WriteOnly);
   Q_UNUSED(ok)
   ts.setDevice(&ct);
   ps.setDevice(&cp);
   for(int i=0; i<wl.size(); i++){
       QString w = wl.at(i).toLower();
       QList<int> *l = cc[w];
       if (!l) showMessage("Nil pointer");
       else {
           qint32 p = ct.pos()/2;   countedMessage(QString::number(p));
           ps << p;
           qint16 q = l->size();
           ts << q;
           for(int j=0; j<l->size(); j++){
               q = l->at(j);
               ts << q;
           }

       }
   }
   ct.close();
   cp.close();

   downloadProgressBar->setVisible(false);

   showMessage(tr("All done"));
};

void BibleWindow::onBDownloadDone(bool e){
   if (e || fileDownloader->notDone){
     showMessage("download error");
     return;
   }
   QStringList a;
   a << "-o" << QFileInfo(zipFile).absoluteFilePath();
//   showMessage(a.join("\n"));
   if (!process){
      process = new QProcess(this);
      connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onUnzipFinished(int,QProcess::ExitStatus)));
   }
   process->setWorkingDirectory(QFileInfo(zipFile).absolutePath());
   process->start("unzip",a);
};

void BibleWindow::onUnzipFinished( int exitCode, QProcess::ExitStatus exitStatus ){
   Q_UNUSED(exitCode); Q_UNUSED(exitStatus);
   showMessage( tr("Downloaging is finished. Now you can open the Bible %1.").arg(windowTitle()) );
   QFile( QFileInfo(zipFile).absoluteFilePath() ).remove();
};

void BibleWindow::globalToLocal(){
   ch = gch; vr = gvr;
   if (!correctionGL.size()) return;
   int i = 0;
   while( (i<correctionGL.size()) && (correctionGL.at(i)->bk < bk) ) i++;
   while( (i<correctionGL.size()) && (correctionGL.at(i)->bk == bk) ){
      if ( (correctionGL.at(i)->ch0 == gch) && (correctionGL.at(i)->vr0 <= gvr) ){
         ch = gch - correctionGL.at(i)->ch1;
//         showMessage(bibleVersion()+" :: " + QString::number(correctionGL.at(i)->ch1)+" "+QString::number(correctionGL.at(i)->vr1));
         vr = gvr - correctionGL.at(i)->vr1;
      }
      i++;
   }
};

void BibleWindow::localToGlobal(){
   gch = ch; gvr = vr;
   if (!correctionLG.size()) return;
   int i = 0;
   while( (i<correctionLG.size()) && (correctionLG.at(i)->bk < bk) ) i++;
   while( (i<correctionLG.size()) && (correctionLG.at(i)->bk == bk) ){
      if ( (correctionLG.at(i)->ch0 == ch) && (correctionLG.at(i)->vr0 <= vr) ){
         gch = ch + correctionLG.at(i)->ch1; 
         gvr = vr + correctionLG.at(i)->vr1;
      }
      i++;
   }
};

QString BibleWindow::verseText(int i){
   int p = pointer(i-1, dir+"CompactPoint.bin");
   if (p<0) return "";
   return textFragment(p, dir+"CompactText.bin", versionCodec(bibleVersion()) );
};

QString between(const QString &s, const QString &e, const QString &st){
   int p1 = st.indexOf(s)+s.size(); if (p1<0) p1 = 0;
   int p2 = st.indexOf(e,p1); if (p2<p1) p2 = st.size()-1;
   return st.mid(p1,p2-p1);
}

void BibleWindow::setVerseColor(int vr, const QString &c1, const QString &c2){
   QTextBlock tb = document()->findBlockByNumber( vr );
   QFont font = tb.charFormat().font();
   setFont(font);
   int p = tb.position();
   if (!p) return;
   QTextCursor tc = textCursor();
   bool up = p>tc.position();
   tc.setPosition(p);
   tc.movePosition(QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
   QString h = tc.selection().toHtml();
   h = between("<!--StartFragment-->","<!--EndFragment-->",h);
   h = h.replace("color:"+c1+";","color:"+c2+";");
   h.replace("text-decoration: underline","text-decoration: none");
   QString nt = "<font color=\""+c2+"\">"+h+"</font>";
   tc.insertHtml(nt);
   if (up) setTextCursor(tc);
   tc.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
   setTextCursor(tc);
};

QStringList *BibleWindow::verseTexts(int i, int c){
   int p = pointer(i-1, dir+"CompactPoint.bin");
   if (p<0) return 0;
   return textFragments(p, c, dir+"CompactText.bin", versionCodec(bibleVersion()) );
};

QString BibleWindow::wordChapter(){
  return wordChapter(book());
};

QString BibleWindow::addTags(const QString &s0){
   int i; int j1 = 0;
   bool f1 = true, f2 = false;
   QString s = s0;
   if (bible_Version=="KjvSn") s.replace("<","&lt;");
   QString r = "";
   for(i = 0; i<s.size(); i++){
      switch (s.at(i).unicode()){
      case 0x7B: // {
         if (i>j1) r += s.mid( j1, i-j1-1 );
         j1 = i+1;
         f2 = true;
         break;
      case 0x7C: // |
         if (f2) break;
         r += s.mid( j1, i-j1 );
         if (f1) r += "<i>"; else r += "</i>"; 
         f1 = !f1;
         j1 = i+1;
         break;
      case 0x7D: // }
         fCount++;
         QString sn = QString::number(fCount);
         switch (preferences()->footnotes()){
         case 0:
            r += "<strong><sup> <a href=\"#f"+sn+"\">"+sn+"</a></sup></strong>";
            footnotes += "<a name=\"f"+sn+"\"></a><p class=\"footnote\"><sup>"+sn+"</sup> " +
               addTags(s.mid( j1, i-j1 ))+"</p>";
            break;
         case 1:
            r += " <font color=\""+preferedColor()->footnoteColor().name()+"\">{"+addTags(s.mid( j1, i-j1 ))+"}</font> ";
            break;
         };
         j1 = i+2;
         f2 = false;
         break;
      }
   }
   r += s.mid(j1,i-j1);
   r = concordance_->colorWords(r);
   return r;
};

QString BibleWindow::addLinks(int i, int v){
   QString r = ""; QString lnk = "";
   if (QFileInfo::exists(dir+"LinksP.bin")){
      int p = pointer(i-1,dir+"LinksP.bin"); 
      if (p>=0){//showMessage(p);
         QString ln = QString::number(p);
         switch (preferences()->references()){
         case 0: // Препратките се поставят в края на главата
            r = " <a href=\"#"+ln+"\"><sup><strong>"+QChar(0x21e8)+"</strong></sup></a>";
            lnk += "<a name=\""+ln+"\"></a><p class=\"footnote\"><sup>"+QString::number(v)+"</sup> ";
            break;
         case 1: // Препратките се поставят в края на стиховете
            lnk = " <font color=\""+preferedColor()->footnoteColor().name()+"\">{"; 
            break;
         };
         QList<int> *ia = indexArray(p, dir+"Links.bin");
         for(int k=0; k<ia->size(); k++){
            int vi = ia->at(k);
            QString t = QString::number(vi);
            lnk += "<a href=\""+t+"\">"+ reference(vi,true)+"</a>";
            if (k==ia->size()-1) lnk += ".";
            else lnk += ", ";
         }
         switch (preferences()->references()){
         case 0: lnk += "</p>"; break;
         case 1: lnk += "}</font>"; break;
         }
      }
   }
   switch (preferences()->references()){
   case 0:
      links += lnk;
      return r;
   case 1:
      return lnk;
   };
   return "";    
};

QString BibleWindow::wordChapter(int b){
  b = global_book.value(b);
  QString ln = language();
  if (b==22)  return languageObject()->psalm(ln);
  else return languageObject()->chapter(ln);
};

void BibleWindow::displayText(){
   if ((bk!=bkl)||(ch!=chl)||wordsChanged) displayFreshText();
   if (!verseCount()) return;
   setVerseColor(vrl, preferedColor()->activeVerseColor().name(), preferedColor()->bibleTextColor().name()  );
   setVerseColor(vr,  preferedColor()->bibleTextColor().name(),   preferedColor()->activeVerseColor().name());
   vrl = vr;
};

void BibleWindow::displayFreshText(){
   clear();
   QPalette p = palette();
   p.setColor(QPalette::Active, QPalette::Base, preferedColor()->baseColor());
   p.setColor(QPalette::Active, QPalette::Text, preferedColor()->bibleTextColor());
   p.setColor(QPalette::Active, QPalette::Link, preferedColor()->footnoteColor());
   setPalette(p);
   bkl=bk;
   if (book()==0){
      setText(reference());
      return;
   }
   chl=ch; 
   int i = referenceToIndex(book(), chapter(), 1, bible_Structure);
   links = ""; footnotes = ""; fCount = 0;
   QString tx = "<html><head><style>"+css+"</style>"+
      "</head><body>\n<h3>"+longTitles.value(book()-1)+"\n"+
      "<br />"+wordChapter()+" &nbsp; "+QString::number(chapter())+"</h3>\n";
   for(int j=0; j<verseCount(); j++){
     QString vtx = verseText(i+j);
     QString sty = "";
     if (vtx.at(0)==QChar(182)){
        vtx = vtx.remove(0,1);
        sty = " class=\"pstart\"";
     }
     if (vtx.size()) tx += "<p"+sty+">"+QString::number(j+1)+". "+ addTags(vtx) + addLinks(i+j,j+1)+"</p>";
     else tx += "<p>&nbsp;</p>";
   }
   if (links.size()) links = "<hr width=\"50%\">"+links;
   if (footnotes.size()) footnotes = "<hr>"+footnotes;
   setHtml(tx+footnotes+links+"</body></html>");
   wordsChanged=false;
};

// Генерира html код за експортиране
// Параметърът e определя какво да се експортира

QString BibleWindow::toHtml(ExportDialog::Export e){
   int b1=1,           // Номер на първата книга за експортиране
       b2=bookCount(); // Номер на последната книга за експортиране
   QString r = "";     // Връщания резултат
   QString ht = windowTitle(); // Зглавие в <title> тага на файла
   // Ако не се акспортира цялата Библия
   if (e!=ExportDialog::Bible){ b1=book(); b2=b1; if (b1) ht = longTitles.value(b1-1); }
   r = "<h1>"+windowTitle()+"</h1>\n\n";
   for(int b=b1; b<=b2; b++){
      // Ако се експортира само текущата книга
      if (b2==b1)
      r += "<h2>"+longTitles.value(b-1)+"</h2>\n\n";
      int c1=1, c2=bible_Structure->value(b-1)->size()-1;
      if (e==ExportDialog::Chapter){ c1=chapter(); c2=c1; ht = wordChapter(b)+" &nbsp; "+QString::number(c1);}
      for(int c=c1; c<=c2; c++){
         r += "<h3>"+wordChapter(b)+" &nbsp; "+QString::number(c)+"</h3>\n\n";
         int i = referenceToIndex(b,c,1,bible_Structure);
         int vc = bible_Structure->value(b-1)->value(c);
         QStringList *tx = verseTexts( i, vc );
         for(int v=0; v<vc; v++){
            r += "<p>"+QString::number(v+1)+". "+addTags( tx->at(v) )+"</p>\n";
         }
      }
   }
   QString t1 = "<!doctype html>\n<html>\n<head>\n";
   r = t1+"<title>"+ht+"</title>\n"+
        "   <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"+
        "   <style>body{font-family: sans-serif;}</style>\n"+
        "</head>\n\n"+
        "<body>\n"+r+"\n</body>\n</html>";
   return r;
};

QString BibleWindow::toTxt(ExportDialog::Export e){
   int b1=1, b2=bookCount();
   QString r = "";
   if (e!=ExportDialog::Bible){ b1=book(); b2=b1; }
   if (!b1) return r;
   r = windowTitle()+"\n\n";
   for(int b=b1; b<=b2; b++){
      r += longTitles.value(b-1)+"\n\n";
      int c1=1, c2=bible_Structure->value(b-1)->size()-1;
      if (e==ExportDialog::Chapter){ c1=chapter(); c2=c1; }
      for(int c=c1; c<=c2; c++){
         r += wordChapter(b)+" "+QString::number(c)+"\n\n";
         int i = referenceToIndex(b,c,1,bible_Structure);
         int vc = bible_Structure->value(b-1)->value(c);
         QStringList *tx = verseTexts( i, vc );
         for(int v=0; v<vc; v++){
            r += QString::number(v+1)+". "+tx->at(v)+"\n";
         }
      }
   }
   return r;
};

void BibleWindow::about(MyProcess *p){
   QFile af( dir+"about.html" );
   if (!af.exists()){
      showMessage( tr("There is no information about this Bible Version.") );
      return;
   }
   p->browse( QFileInfo(af).absoluteFilePath() );
};

ConcordanceModel *BibleWindow::concordance(){
   setFileNOpened(false);
   return concordance_;
};

int BibleWindow::readPos(){
   int r = read_pos.value(bibleVersion());
   if (!r) return 1;
   else return r;
};

void BibleWindow::setReadPos(){
   if (verseIndex()==readPos()+1) read_pos[bibleVersion()] = verseIndex();
};

QString BibleWindow::language(){
   return versionLanguage(bibleVersion());
};

QString BibleWindow::bibleVersion(){
   return bible_Version;
};

QString BibleWindow::bibleDir(){
//   return QFileInfo(dir).absolutePath();
   return dir;
};

void BibleWindow::setReference(int b, int c, int v){
  bk = global_book.value(b+1); 
  if (c<0) ch = 1; else ch = c+1;
  if (v<0) vr = 1; else vr = v+1;
};

int BibleWindow::verseIndex(){
   return referenceToIndex(book(),chapter(),verse(),bible_Structure);
};

int BibleWindow::verseIndex(int b, int c, int v){
   return referenceToIndex(b,c,v,bible_Structure);
};

int BibleWindow::verseIndex(const QString &s){
   QStringList l1 = s.split(":");
   static int b0 = 1, c0 = 1; // Номер на книга и глава от последното извикване на функцията
   bool ok;    // Ще се използва в toInt(&ok)
   int b,v,c;  // Променливи за номера на книга, глава, стих.
   if (l1.size()<2){ // Препратката не съдържа знак :
      v = l1.at(0).toInt(&ok); // Ако е само число, трябва да е номер на стих от книга b0 глава c0
      if (!ok) { showMessage(tr("Can't recognize reference %1").arg(s)); return -1; }
      return referenceToIndex(b0,c0,v,bible_Structure);
   }
   QStringList l2 = l1.at(0).split(" "); // Частта преди : се разделя на части, отделени с интервали.
   QString bn; // Променлива, предназначена за съкратеното име на книгата
   v = l1.at(1).toInt(&ok); // Определяне номера на стиха
   if (!ok) { // Ако номера на стих не е число - съобщение за грешка
     showMessage(tr("Can't recognize verse number in reference %1").arg(s));
     return -1;
   }
   switch (l2.size()){ // 
   case 1: // Този случай означава, че първата част е само номер на глава или само име на книга.
      bn = l2.at(0).trimmed();
      c = bn.toInt(&ok);
      if (ok){ // Ако е номер на глава, се използва последно определения номер на книга b0.
        c0 = c;
        return referenceToIndex(b0,c,v,bible_Structure);
      }
      b = book_index.value(bn);
      if (!b){ // Ако не е име на книга - съобщение за грешка
         showMessage(tr("Can't recognize book in reference %1").arg(s)); return -1;
      }
      showMessage( tr("The case of reference like %1 is not programed.").arg(s) ); return -1;
      break;
   case 2: // Към този случай се отнасят книгите с една дума в съкратеното си заглавие
      bn = l2.at(0).trimmed();
      b = book_index.value(bn);
      c = l2.at(1).toInt(&ok);
      if (!b || !ok) {
        showMessage(tr("Can't recognize reference %1").arg(s)); return -1;
      }
      break;
   case 3: // Към този случай се отнасят книгите, на които съкратеното заглавие се състои от две части
      bn = l2.at(0).trimmed()+" "+l2.at(1).trimmed();
      b = book_index.value(bn);
      c = l2.at(2).toInt(&ok);
      if (!b || !ok) {
        showMessage(tr("Can't recognize reference %1").arg(s)); return -1;
      }
      break;
   case 4: // Към този случай се отнасят книгите, на които съкратеното заглавие се състои от три части
      bn = l2.at(0).trimmed()+" "+l2.at(1).trimmed()+" "+l2.at(2).trimmed();
      b = book_index.value(bn);
      c = l2.at(3).toInt(&ok);
      if (!b || !ok) {
        showMessage(tr("Can't recognize reference %1").arg(s)); return -1;
      }
      break;
   default:
      showMessage(tr("Can't recognize reference %1").arg(s)); return -1; 
   }
   b0 = b, c0 = c;
   return referenceToIndex(b,c,v,bible_Structure);
};

// Задаване на стих по индекс.
void BibleWindow::setVerseByIndex(int i){
   int b,c,v;
   indexToRefrence(i,&b,&c,&v,bible_Structure);
   setReference(b-1,c-1,v-1);
};

void BibleWindow::globalIndexToLocal(){
   indexToRefrence(globalVerseIndex(), &bk, &gch, &gvr, globalStructure() );
//   showMessage("Global:   "+bibleVersion()+" :: " + QString::number(bk)+":"+QString::number(gch)+":"+QString::number(gvr));
   globalToLocal();
//   showMessage("Local:   "+bibleVersion()+" :: " + QString::number(bk)+":"+QString::number(ch)+":"+QString::number(vr));
};

int BibleWindow::localToGlobalIndex(){
   localToGlobal(); //showMessage("localToGlobalIndex");
   return referenceToIndex(bk, gch, gvr, globalStructure() );
};

int BibleWindow::book(){
   return local_book.value(bk);
};

int BibleWindow::book(int b){
   return local_book.value(b);
};

int BibleWindow::globalBook(){
   return bk;
};

int BibleWindow::globalBook(int b){
   return global_book.value(b);
};

int BibleWindow::chapter(){
   return ch;
};

int BibleWindow::verse(){
   return vr;
};

int BibleWindow::bookCount(){
   return bible_Structure->size();
};

int BibleWindow::chapterCount(){
   if (!book()) return 0;
   return bible_Structure->value(book()-1)->size()-1;
};

int BibleWindow::chapterCount(int b){
   if (!b) return 0;
   return bible_Structure->value(b-1)->size()-1;
};

int BibleWindow::verseCount(){
   if (!book()) return 0;
   return bible_Structure->value(book()-1)->value(chapter());
};

int BibleWindow::verseCount(int c){
   if (!book()) return 0;
   return bible_Structure->value(book()-1)->value(c);
};

int BibleWindow::verseCount(int b, int c){
   if (!b) return 0;
   return bible_Structure->value(b-1)->value(c);
};

int BibleWindow::verseTotalCount(){
   static int tc = 0;
   if (!tc)
      for(int b=0; b<bookCount(); b++){
         for(int c=0; c<chapterCount(b+1); c++)
            tc += verseCount(b+1,c+1);
       }
   return tc;
};

QString BibleWindow::reference(bool abr){
   QString r = tr("No such book in this version.");
   if (book()==0) return r;
   if (!abr) r = shortTitles.at(book()-1);
   else r = abrevs.at(book()-1);
   r = r + " " + QString::number(chapter())+":"+QString::number(verse());
   return r;
};

QString BibleWindow::reference(int i, bool abr){
   int b = book(), c=chapter(), v=verse(); 
   indexToRefrence(i,&b,&c,&v,bible_Structure);
   if ( (c>chapterCount(b)) || (v>verseCount(b,c)) ) return tr("Error");
   else {
      QString r = "";
      if (!abr) r = shortTitles.at(b-1);
      else r = abrevs.at(b-1);
      return r+" "+QString::number(c)+":"+QString::number(v);
   }
};

void BibleWindow::onGlobalIndexChanged(BibleWindow *bw){
   if (!synchronize) return;
   if (bw!=this) globalIndexToLocal();
   displayText();
};

void BibleWindow::scrollToActiveVerse(){
   int p = document()->findBlockByNumber( verse() ).position();
   if (!p) return;
   QTextCursor tc = textCursor();
   tc.setPosition(p);
   setTextCursor(tc);
   tc.movePosition(QTextCursor::EndOfBlock,QTextCursor::MoveAnchor);
   setTextCursor(tc);
};

void BibleWindow::refreshText(){
   displayFreshText();
   setVerseColor(vr,preferedColor()->bibleTextColor().name(), preferedColor()->activeVerseColor().name());
   scrollToActiveVerse();
};

void BibleWindow::closeEvent(QCloseEvent *event){
   Q_UNUSED(event);
   writeSettings();
   emit closing(this);
};

//------------------------------------------------------

int global_Verse_index = 1;
QHash<QString, QString> version_Caption, version_Codec, version_Language;
BibleStructure *global_structure;

QString versionCaption(const QString &bv){
   return version_Caption[bv];
};

QByteArray versionCodec(const QString &bv){
   return version_Codec[bv].toUtf8();
};

QString versionLanguage(const QString &bv){
   return version_Language[bv];
};

void setVersionAttributes(const QString &bv, const QString &bc, const QString &vc, const QString &vl){
   version_Caption[bv] = bc;
   version_Codec[bv] = vc;
   version_Language[bv] = vl;
};

void readGlobalStructure(){
   global_structure = readStructure(bible_Path+"Bible_Structure.txt");
};

BibleStructure *readStructure(const QString &fn){
   QString fc = fileContent(fn);
   QStringList fl = fc.split("\n");
   BibleStructure *r = new BibleStructure;
   r->verseCount = 0;
   for(int i=0; i<fl.size(); i++){
      if (!fl.at(i).size()) break;
      QStringList l1 = fl.at(i).trimmed().split(" ");
      QList<int> *n = new QList<int>;
      int vc = 0;
      for(int j=1; j<l1.size(); j++){
         int k = l1.at(j).toInt();
         vc += k;
         n->append(k);
      }
      n->insert(0,vc);
      r->verseCount += vc;
      r->append(n);
   }
   return r;
};

BibleStructure *globalStructure(){
   return global_structure;
};

int globalVerseIndex(){
   return global_Verse_index;
};

void setGlobalVerseIndex(int i){
   global_Verse_index = i;
};

void indexToRefrence(int index, int *bk, int *ch, int *vr, const BibleStructure *bs){
   int b=1, c=1, vc = bs->value(0)->value(0);
   while((b<bs->size()) && (index>vc)){
     index -= vc; b++;
     QList<int> *il = bs->value(b-1);
     if (il) vc = il->value(0);
   }
   vc = bs->value(b-1)->value(c);
   while((c<bs->value(b-1)->size()) && (index>vc)){
      index -= vc;
      c++;
      vc = bs->value(b-1)->value(c);
   }
   *bk = b; *ch = c; *vr = index;
};

int referenceToIndex(int bk, int ch, int vr, const BibleStructure *bs){
  int r = 0;
  if (!bk) return 0;
//  showMessage(QString::number(bk)+" "+QString::number(ch)+" "+QString::number(vr)+" ");
//  if (bk<1) bk=1;
  for(int i=0; i<bk-1; i++) r += bs->value(i)->value(0);
  for(int i=1; i<ch; i++) r += bs->value(bk-1)->value(i);
  return r+vr;
};

int pointer(int i, const QString &fn){
   QFile file(fn);
   if (!file.open(QFile::ReadOnly)) return -1;
   QDataStream ds(&file);
   ds.setByteOrder(QDataStream::LittleEndian);
   file.seek(i*4);
   qint32 r;
   ds >> r;
   file.close();
   return r;
};

int pointer2(int i, const QString &fn){
   QFile file(fn);
   if (!file.open(QFile::ReadOnly)) return -1;
   QDataStream ds(&file);
   ds.setByteOrder(QDataStream::LittleEndian);
   file.seek(i*2);
   quint16 r;
   ds >> r;
   file.close();
   return r;
};

QString textFragment(int p, const QString &fn, const QString &cd){
   QFile file(fn);
   if (!file.open(QFile::ReadOnly)) return "";
   QDataStream ds(&file);
   ds.setByteOrder(QDataStream::LittleEndian);
   file.seek(p);
   quint16 r;
   ds >> r; //countedMessage(QString::number(p)+" "+QString::number(r));
   QByteArray b;
   b.resize(r);
   ds.readRawData(b.data(),r);
   file.close();
   QString rt = myDecode(b,cd);
   return rt;
};

QStringList *textFragments(int p, int c, const QString &fn, const QString &cd){
   QFile file(fn);
   if (!file.open(QFile::ReadOnly)) return 0;
   QStringList *sl = new QStringList();
   QDataStream ds(&file);
   ds.setByteOrder(QDataStream::LittleEndian);
   file.seek(p);
   do{
      quint16 r;
      ds >> r;
      QByteArray b;
      b.resize(r);
      ds.readRawData(b.data(),r);
      b[r]=0;
      QString rt = myDecode(b,cd);
      if (rt.size()==0) showMessage("Null string");
      sl->append(rt);
      c--;
   } while(c>0);
   file.close();
   return sl;
};

void setDownloadWidgets(QProgressBar *pb, QPushButton *cb){
   downloadProgressBar = pb;
   dounloadCancelButton = cb;
};

QString download_site = "physics-bg.org";

void setDownloadSite(const QString &ds){
   download_site = ds;
};

QString downloadSite(){
   return download_site;
}

QString default_Css = "";

void setDefaultCss(const QString &ds){
   default_Css = ds;
};

QString defaultCss(){
   return default_Css;
};

QList<int> *indexArray(int i, const QString &fn){
   QList<int> *r = new QList<int>;
   if (i<0) return r;
   QFile file(fn);
   if (!file.open(QFile::ReadOnly)) return r;
   QDataStream ds(&file);
   ds.setByteOrder(QDataStream::LittleEndian);
   quint16 a;
   file.seek(i*2);
   ds >> a;
   while (a>0){
      quint16 b;
      ds >> b;
      r->append(b);
      a--;
   }
   file.close();
   return r;
};

QString biblePath(){
   return bible_Path;
};

void setBiblePath(const QString &s){
  bible_Path = s;
};

QStringList getReadPositions(){
   QStringList k = read_pos.keys();
   QStringList r;
   for(int i=0; i<k.size(); i++) r << k.at(i) + "=" + QString::number( read_pos.value(k.at(i)) );
   return r;
};

void setReadPositions(const QStringList &bl){
   for(int i=0; i<bl.size(); i++){
       QStringList l = bl.at(i).split("=");
       read_pos[l.at(0)] = l.at(1).toInt();
   }
};
