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

Този модул съдържа класове, необходими за отваряне и показване текста
на резлични издания на Библията.

*/

// Класове, свързани с показване текста на Библията

#ifndef BIBLEWINDOW_H
#define BIBLEWINDOW_H

#include "exportDialog.h"

#include <QAbstractListModel>
#include <QTextBrowser>
#include <QHash>
#include <QProcess>

class QFile;
class QProgressBar;
class QPushButton;
class FileDownloader;
class MyProcess;
class ConcordanceModel;

/* -------BibleStructure---------
   Клас за описване структурата на Библията - 
   колко глави има всяка книга, и колко стиха има всяка глава.
   Понеже има разлики между различните издания на Библията,
   за установяване съответствието между стиховете, се използват следните понятия:
   Глобална структура се нарича структурата на едно несъществуващо издание на
   Библията, в което: има толкова книги, колкото има в изданието с най-много книги;
   всяка книга има толкова глави колкото в изданието с най-много глави в тази книга;
   всяка глава има толкова стихове, колкото в изданието с най-много стихове в тази глава.
   Локална структура се нарича структурата на конкретно издание на Библията.
   Индекс на стих е поредния номер на даден стих, отчетен от началото на Библията.
   Всеки стих има локален и глобален индекс. Локалният индекс се пресмята според локалната
   структура, а глобалния - според глобалната.
*/

class BibleStructure : public QList<QList<int>*>
{
public: 
   int verseCount; // Общ брой на стиховете в конкретното издание
};

/*--------VerseCorrection---------
   Клас за описване на корекциите за преминаване от локална към глобална структура
   и обратно.
*/

class VerseCorrection: public QObject{
public:
  VerseCorrection(const QStringList &cl);
  int bk, // Глобален номер на книга
  ch0, // Локален номер на глава
  vr0, // Локален номер на стих, от който нататък, до края на главата, трябва да се направи корекция.
  ch1, // Корекция на локалния номер на глава
  vr1;  /* Корекция на локалния номер на стиха
     При преход от локални към глобални номера, корекцията се прибавя.
     При преход от глобални към локални номера, корекцията се изважда.
     */
};

/*-----------VerseCorrectionList---------
   Списък на корекциите за дадено издание на Библията.
   Чете се от файл _Diff_.txt. Всеки ред от този файл съдържа четирите полета за class VerseCorrection.
*/

class VerseCorrectionList : public QList<VerseCorrection*>{};

/*-----------BibleWindow---------------
   Клас, представляващ прозорец, в който се показва една глава от дадено
   издание на Библията. Класът съдържа и друга информация за самото издание.
*/

class BibleWindow: public QTextBrowser
{
   Q_OBJECT

public:
   QStringList longTitles, // Пълни заглавия на книгите, както се показват в текста.
   shortTitles, // Кратки заглавия на книгите, както се показват в менюто.
   abrevs; /* Съкращения на заглавията на книгите.
      Тези три форми на заглавията се прочитат от файл BibleTitles.txt от директорията
      на съответното издание.
   */
   bool synchronize,  // Определя дали прозорецът да се синхронизира с активния стих
   wordsChanged; // Става истина при смяна на думите за търсене

   BibleWindow( const QString &bv, QWidget *parent = 0); // Конструктор на класа. bv е идентификатор на версия на Библията

   QString bibleVersion(); /* Идентификатор на версията на Библията, който съвпада с името на директорията й.*/
   QString bibleDir(); // Връща пътя на директорията с файловете от дадената Библия
   void setReference(int b, int c, int v); /* Задаване номерата на текущите книга, глава и стих.*/
   int verseIndex(); // Пресмята локалния индекс на текущия стих
   int verseIndex(int b, int c, int v); // Пресмята локалния индекс на текущия стих
   int verseIndex(const QString &s); // Пресмята локалния индекс на текущия стих, зададен с текстова препратка s.
   void setVerseByIndex(int i); // Задава текущ стих по локален индекс
   void globalIndexToLocal(); // Трансформация от глобален към локален индекс на стих 
   int localToGlobalIndex(); // Трансформация от глобален към локален индекс на стих
   int book(); // Локален номер на текущата книга
   int book(int b); // Локален номер на книгата с глобален номер b
   int globalBook(); // Глобавен номер на текущата книга
   int globalBook(int b); // Глобавен номер книга с локален номер b
   int chapter(); // Локален номер на текущата глава
   int verse(); // Локален номер на текущия стих
   int bookCount(); // Брой на книгите от даденото издание на Библията
   int chapterCount(); // Брой на главите в текущата книга
   int chapterCount(int b); // Брой на главите в книга b
   int verseCount(); // Брой на стиховете в текущата глава
   int verseCount(int c); // Брой на стиховете в глава c на текущата книга
   int verseCount(int b, int c); // Брой на стиховете в книга b, глава c
   int verseTotalCount(); // Брой на всички стихове от Библията
   QString reference(bool abr = false); // Препратка за текущия стих, когато abr=true, името на книгата е съкратено
   QString reference(int i, bool abr = false); // Връща припратка към стих с локален индекс i. Когато abr=true името на книгата е съкратено
   QString verseText(int i); // Връща текста на i-ия стих.
   void displayText(); // Опресняване на текста в прозореца
   void displayFreshText(); // Четене и показване на нов текст
   QString toHtml(ExportDialog::Export e); // Част от текста на Библията в html формат
   QString toTxt(ExportDialog::Export e); // Част от текста на Библията в txt формат
   void about(MyProcess *p); // Отваря "Относно" информацията за Библията
   ConcordanceModel *concordance(); // Модел на списъка с думи. Използва се в конкорданса.
   int readPos(); // Връща номера на стиха, достигнат с последователно четене.
   void setReadPos(); // Запазва номера на стиха, достигнат с последователно четене.
   QString language(); // Връща иентификатор на езика
   void downloadBible(const QString &bv); // Изтегля от Интернет и разархивира Библия bv
   void writeSettings(); // Записва настройките
   void readSettings(); // Чете настройките
   void createLinksBin(); // Създава файловете Links.bin и LinksP.bin
   void import(const QString &fn); // Импортиране на Библия от .txt файл.
signals:
   void closing(BibleWindow *bw); // Сигнал, който се излъчва при затваряне на прозореца.

public slots:
   void onGlobalIndexChanged(BibleWindow *bw); // Слот, който прихваща промяната на текущия стих
   void scrollToActiveVerse(); // "Превърта" прозореца така че да се вижда активния стих
   void refreshText(); // Показва отново текста в прозореца след промяна на опция, засягаща нечина на показване

protected:
   void closeEvent(QCloseEvent *event); // Функция, която се изпълнява при събитие затваряне на прозореца.

private slots:
   void onBDownloadDone(bool e); // Приема сигнал за край на тегленето от Интернет
   void onUnzipFinished( int exitCode, QProcess::ExitStatus exitStatus ); /* Приема сигнал, че е завършено разархивирането
      на изтегления zip файл */

private:
   QString bible_Version, // Идентификатор на версията на Библията, който съвпада с името на директорията й.
     dir, // Пълно име до директорията с файлове от дадената версия.
     zipFile, // Пълно име на изтегления от Интернет zip файл
     css, // Стил (CSS)
     links, // Препратки
     footnotes; // Бележки под линия
   int book_count, // Брой на книгите в дадената версия на Библията
      bk, // Глобален номер на текущата книга
      ch, // Локален номер на текущата глава
      vr, // Локален номер на текущия стих
      gch, // Глобален номер на текущата глава
      gvr, // Глобален номер на текущия стих
      bkl, chl, vrl, // Променливи, в които се записват стойностите на bk, ch и vr след промяната им.
         // Служат за откриване дали са настъпили промени в стойностите им.
      fCount; // Брой на бележките под линия към текущата глава.
   QHash<int, int> global_book,  // Хеш за бързо преминаване от локални към глобални номера на книгите.
      local_book;  // Хеш за бързо преминаване от глобални към локални номера на книгите.
         // Информацията за това съответствие е в първия ред на файл BibleTitles.txt
   QHash<QString,int> book_index; // Хеш за откриване номера на книгата по съкратеното й заглавие. 
   BibleStructure *bible_Structure; // Локална структура на Библията
   VerseCorrectionList correctionLG, // Списък на корекциите от локални към глабални означения
      correctionGL; // Списък на корекциите от глобални към локални означения
   FileDownloader *fileDownloader; // Обект, който изтигля файл от Интернет
   QProcess *process;
   ConcordanceModel *concordance_;

   bool readTitles(); // Четене на заглавията от файл BibleTitles.txt
   void readCorrection(); // Четене на корекциите от файл _Diff_.txt
   void globalToLocal(); // Пресмята стойности на gch и gvr, съответстващи не текущите стойности на ch и vr.
   void localToGlobal(); // Пресмята стойности на ch и vr, съответстващи не текущите стойности на gch и gvr.
   void setVerseColor(int vr, const QString &c1, const QString &c2); // Оцветява i-я стих във цвят c (цвета се задава като в html формат)
   QStringList *verseTexts(int i, int c); // Чете текста на c стихове, започвайки от i-ия.
   QString wordChapter(); // Връща думите "Глава" или "Псалом" на съответния език
   QString wordChapter(int b); // Връща думите "Глава" или "Псалом" на съответния език
   QString addTags(const QString &s); /* Вмъква html тагове за да се форматира текста, защото в текстовите фрагменти
      текста, който трябва да е italic e заграден с вертикални черти |, а bold - в обратно наклонени черти \.*/
   QString addLinks(int i, int v); // Добавя препратки, ато те са описани в отделни файлове Links.bin и LinksP.bin.
};

//----------функции-------------

QString versionCaption(const QString &bv); // По идентификатора на версията връща заглавието й.

QByteArray versionCodec(const QString &bv); // По идентификатора на версията връща кодовата й таблица.

QString versionLanguage(const QString &bv); // По идентификатора на версията връща езика й.

void setVersionAttributes(const QString &bv, const QString &bc,  const QString &vc, const QString &vl); 
/* Запазва информацията за версия на Библията с идентификатор bv.
    А именно: заглавието bc, кодовата таблица vc и език vl.
   Тази информация се намира в редовете на файл data/bibles/list.txt и се съхранява в хешове.
   (Виж реализацията на функцията в bibleWindow.cpp) */

void readGlobalStructure(); // Чете глобалната структура от файл data/bibles/BibleStructure.txt.

BibleStructure *readStructure(const QString &fn); // Чете структура от файл fn

BibleStructure *globalStructure(); // Връща глобалната структура

int globalVerseIndex(); // Връща глобалния индекс на текущия стих

void setGlobalVerseIndex(int i); // Задава стойност на глобалния индекс

void indexToRefrence(int index, int *bk, int *ch, int *vr, const BibleStructure *bs); /* Пресмята съответстващите
   на стойността на индекса i, стойности на номерата на книгата bk, главата ch и стиха vr,
   съответстващи на структура на Библията, описана в bs */

int referenceToIndex(int bk, int ch, int vr, const BibleStructure *bs); /* Връща индекса на стиха, съответстващ
   на номерата на книгата bk, главата ch и стиха vr, при структура на Библията bs. */

int pointer(int i, const QString &fn); // Чете i-ия четирибайтов указател от файла с указатели fn
int pointer2(int i, const QString &fn);// Чете i-то двубайтово число от файла с указатели fn

QString textFragment(int p, const QString &fn, const QString &cd = "cp1251"); /* Чете текстов фрагмент,
   посочен от указателя p във файла fn, при кодировка cd. 
   Текстовият фрагмент е структура, която съдържа първо двубайтово число, което показва дължината на стринг и
   след това толкова байтове, колкото са необходими за представяне на текст с тази дължина, при
   определена кодировка. Във файл CompactText.bin в текстови фрагменти са записани един след друг стиховете
   от дадена версия на Библията. Указателите, които посочват къде започва текстовия фрагмент на всеки стих са
   във файл CompactPoint.bin.*/

QStringList *textFragments(int p, int c, const QString &fn, const QString &cd = "cp1251"); /* Чете от позиция
   посочена от указателя p, c на брой текстови фрагменти, от файл fn, с кодировка cd. */

void setDownloadWidgets(QProgressBar *pb, QPushButton *cb); /* Задава индикаторна лента и бутон за прекратяване,
   които се показват при теглене от Интернет. */

void setDownloadSite(const QString &ds);  // Съхранява домейна на сайта, от който става теглене на файлове

QString downloadSite();  // Връща домейна на сайта, от който става теглене на файлове

void setDefaultCss(const QString &ds);  // Задава подразбиращ се стил (CSS)

QString defaultCss(); // Връща подразбиращия се стил (CSS)

QList<int> *indexArray(int i, const QString &fn); /* чете от позиция i на файл с име fn масив от двубайтови
   цели числа */

QString biblePath(); // връща пътя до директорията с библии
void setBiblePath(const QString &s); // запазва пътя до директорията с библии

QStringList getReadPositions(); /* Връща във вид на списък стрингове локалните номера на стиховете,
   до които потребителя е стигнал с последователно четене на всяка Библия */
void setReadPositions(const QStringList &bl); /* Преобразува от стрингове в цели числа и запазва 
   в хеша read_pos локалните номера на стиховете, на местата на последователно четене.
   Тези две функции се използват от функциите writeSettings и readSettings. */
   
#endif
