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

#ifndef CONCORDANCE_H
#define CONCORDANCE_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>
#include <QList>

#include "bibleWindow.h"

class QComboBox;
class QListView;
class MyListView;
class QStringListModel;
class QPushButton;

//-----------------------------

void setFileNOpened(bool b);

/*-------ConcordanceModel-----
   Клас, осъществяващ достъп до индекса на думите и бързо търсене (конкорданс).
   Обект от този клас има във всеки прозорец с Библия.
*/
class ConcordanceModel : public QAbstractListModel{
public:
   QList<int> *iArray; // Списък с локалните номера на стиховете, в които се срещат потърсените думи
   BibleWindow *bible; // Указател към Библия
   QStringList words;  // Списък на потърсените думи
   QList<bool> searchIn; // Списък от флагове в кои книги да се търси

   ConcordanceModel( const QString &pth, const QString &cd, BibleWindow *parent = 0);

   int rowCount( const QModelIndex &parent = QModelIndex() ) const; // Предефинирана функция
   QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const; // Предефинирана функция
   
   QString word(int i) const; // Дума с номер i от списъка с думи в Библията
   int wordCount() const;     // Брой на думите в Библията
   int count(int i) const;    // Предефинирана функция. Връща същото като wordCount()
   QStringListModel *verseModel();  // Модел на данни за показване на списъка с намерени стихове
   int indexOf(const QString &w);   // Намира номера на думата w
   void setIndexArray(int i);
   QList<int> *verseIndexes(int i);
   QList<int> *verseIndexes(const QString &w);
   QString colorWords(const QString &s);
   void writeSettings();
   void readSettings();
   
private:
   QString path;
   QString codec;
   
   QList<int> *filter(QList<int> *l); 
};

/*-------Concordance--------
   Този клас осигурява интерфейс за търсене по дами.
   Обект от този клас се създава от главния прозорец на програмата mainWindow.
   Съдържа обект от клас ConcordanceModel, който се сменя при смяна на активния
   прозорец с Библия, с този от активния прозорец.
*/
class Concordance: public QObject
{
  Q_OBJECT

public:
  Concordance(QObject *parent, QComboBox *cb, QListView *wlv, MyListView *vlv, QPushButton *pb);

  void setModel(ConcordanceModel *cm);

private slots:
  void onWLClicked(const QModelIndex &i);     // Прихваща щракване върху списъка с думи
  void onVLClicked(const QModelIndex &i);     // Прихваща щракване върху списъка с намерени стихове
  void onEditTextChanged(const QString &t);   // Прихваща промяна в текста на полето за писане на думи
  void onGlobalIndexChanged(BibleWindow *bw); // Прихваща промяната на текущия стих
  void onPushButton();                        // Прихваща щракване на бутона "в книгите..."
  void onBibleClosing(BibleWindow *bw);       // Прихваща сигнал closing от прозореца с Библия
  void onCopyList(int it);  // Прихваща щракване върху "Копиране на препратките" в изскачащото меню на списъка с намерени стихове

signals:
   void verseClicked(BibleWindow *bw, int i);

private:
   QComboBox *comboBox;
   QListView *wListView;
   MyListView *vListView;
   QPushButton *pushButton;
   ConcordanceModel *cModel;
   QString vDel; // Разделител на стиховете при копирането им в Clipboard

   void fillVList(const QStringList &sl);
};

#endif
