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

#include <QFileInfo>
#include <QTextStream>
#include <QModelIndexList>
#include <QListView>
#include <QStringListModel>
#include <QComboBox>
#include <QPushButton>
#include <QSettings>
#include <QInputDialog>
#include <QClipboard>

#include "concordance.h"
#include "myListView.h"
#include "showMessage.h"
#include "preferences.h"
#include "filterDialog.h"

bool fileNOpened = false;

void setFileNOpened(bool b){
   fileNOpened = b;
};

//-----------------ConcordanceModel---------

ConcordanceModel::ConcordanceModel(const QString &pth, const QByteArray &cd, BibleWindow *parent)
   :QAbstractListModel(parent)
{
   path = pth;
   codec = cd;
   bible = parent;
   iArray = 0;
   fileNOpened = false;
   for(int i=0; i<bible->bookCount(); i++) searchIn.insert(i,true);
   readSettings();
};

int ConcordanceModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
   return wordCount();
};

QVariant ConcordanceModel::data( const QModelIndex & index, int role ) const
{
   if (!index.isValid())
      return QVariant();
   if (index.row() >= rowCount())
      return QVariant();
   if (role == Qt::DisplayRole){
      int i = index.row();
      return word(i) + " " + QString::number(count(i));
   }
   else
      return QVariant();
};


QString ConcordanceModel::word(int i) const
{
   int p = pointer(i,path+"WordPoint.bin");
   QFile file(path + "WordList.txt");
   fileNOpened = !file.open(QFile::ReadOnly);
   if (fileNOpened) return "";
   QTextStream ds(&file);
   ds.setCodec(codec);
   if (!ds.seek(p)) return "seek() not done";
   QString r = ds.readLine();
   file.close();
   return r;
};

int ConcordanceModel::wordCount() const
{
   if (fileNOpened) return 0;
   else return QFileInfo(path+"WordPoint.bin").size() / 4;
};

int ConcordanceModel::count(int i) const
{
   int p = pointer( i,path+"ConcP.bin");
   int r = pointer2(p,path+"Conc.bin");
   return r;
};

QStringListModel *ConcordanceModel::verseModel(){
   QStringList sl;
   for(int i=0; i<iArray->size(); i++){
      QString rf = bible->reference(iArray->at(i));
      sl << rf;
   }
   return new QStringListModel(sl);
};

int ConcordanceModel::indexOf(const QString &w){
   int i2=wordCount()-1;
   if (i2<0) return -1;
   int i1=0;
   QString w0 = w.toLower();
   while (i2-i1>1) {
      int i = (i1+i2)/2;
      if (word(i).toLower()>w0) i2=i;
      else i1=i;
   }
   QString w1 = word(i1).toLower();
   if (w0==w1) return i1;
   QString w2 = word(i2).toLower();
   if (w0==w2) return i2;
   return i2;
};

void ConcordanceModel::setIndexArray(int i){
   iArray = verseIndexes(i);
   words << word(i);
   bible->wordsChanged=true;
};

QList<int> *ConcordanceModel::verseIndexes(const QString &w){
   int i = indexOf(w);
   return verseIndexes(i);
};

QList<int> *ConcordanceModel::verseIndexes(int i){
   QString cp = path + "ConcP.bin";
   QString cd = path + "Conc.bin";
   int p = pointer(i,cp);
   return filter(indexArray(p,cd));
};

QString colorWord(const QString &w, const QString &s){
   QString s0=s;
   int p=-1;
   do {
      p = s0.indexOf(w,p+1,Qt::CaseInsensitive);
      if ((p>-1)
           && !( (p>0) && s0.at(p-1).isLetter() ) 
           && !( (p<s0.size()-w.size()) && s0.at(p+w.size()).isLetter() ) 
          )
      {
         QString t1 = "<font color=\""+preferedColor()->foundWordColor().name()+"\">";
         s0 = s0.replace(p, w.size(), t1+w+"</font>");
         p += t1.size();
      }
   } while (p>-1);
   return s0;
}

QString ConcordanceModel::colorWords(const QString &s){
   QString s0 = s;
   for(int i=0; i<words.size(); i++) s0 = colorWord(words.at(i),s0);
   return s0;
};

// Записване на настройките
void ConcordanceModel::writeSettings(){
   QSettings s("VanyoG", "CD Bible 5");
   QString v="";
   for(int i=0; i<searchIn.size(); i++)
     if (searchIn.at(i)) v.append("1"); else v.append("0");
   QString n = bible->bibleVersion()+"_searchInBooks";
   s.setValue(n,v);
//   showMessage(n+"="+v);
};

// Четене на настройките
void ConcordanceModel::readSettings(){
   QSettings s("VanyoG", "CD Bible 5");
   QString n = bible->bibleVersion()+"_searchInBooks";
   QString v=s.value(n).toString();
   for(int i=0; i<v.size(); i++) searchIn.replace(i, v.at(i)==QChar('1') );
//   showMessage(n+"="+v);
};

QList<int> *ConcordanceModel::filter(QList<int> *l){
   int b=0, min=0, max=0;
   QList<int> *r = new QList<int>;
   for(int i=0; i<l->size(); i++){
     int v = l->at(i);
     while (max<v){
        min = max;
        b++;
        for(int c=1; c<=bible->chapterCount(b); c++) max += bible->verseCount(b,c);
     }
     if (searchIn.at(b-1)) r->append(v);
   }
   return r;
};

//-----------------Concordance--------------

Concordance::Concordance(QObject *parent, QComboBox *cb, QListView *wlv, MyListView *vlv, QPushButton *pb)
  :QObject(parent)
{
   comboBox = cb;
   wListView = wlv;
   vListView = vlv;
   pushButton = pb;
   cModel = 0;
   vDel = "\n";
   connect(wListView, SIGNAL(clicked(QModelIndex)), this, SLOT(onWLClicked(QModelIndex)));
   connect(vListView, SIGNAL(clicked(QModelIndex)), this, SLOT(onVLClicked(QModelIndex)));
   connect(vListView, SIGNAL(popupClicked(int)), this, SLOT(onCopyList(int)));
   connect(comboBox, SIGNAL(editTextChanged(QString)), this, SLOT(onEditTextChanged(QString)));
   connect(pushButton, SIGNAL(clicked()), this, SLOT(onPushButton()));
};

void Concordance::setModel(ConcordanceModel *cm){
   static QString ln;
   QString nl = cm->bible->language(); 
   if (ln != nl) {
      comboBox->setEditText(0);
      ln = nl;
   }
   cModel = cm;
   wListView->setModel(cm);
   vListView->setModel(0);
   cModel->iArray=0;
   onEditTextChanged(comboBox->currentText());
   connect(cm->bible, SIGNAL(closing(BibleWindow*)), this, SLOT(onBibleClosing(BibleWindow*)));
};

QList<int> *intersect(QList<int> *a, QList<int> *b){
   QList<int> *r = new QList<int>;
   if (!a || !b) return r;
   if (!a->size() || !b->size()) return r;
   int i = 0, j = 0;
   while ( (i<a->size()) && (j<b->size()) ){
     if (a->at(i)<b->at(j)) i++;
     else if (a->at(i)==b->at(j)) {
            r->append( a->at(i) );
            i++; j++;
          }
          else j++;
   };
   return r;
}

void Concordance::fillVList(const QStringList &sl){
  cModel->words.clear();
  QList<int> *vi = new QList<int>;
  for(int i=0; i<sl.size(); i++){
     cModel->words << sl.at(i);
     cModel->bible->wordsChanged=true;
     if (i==0) vi = cModel->verseIndexes(sl.at(i));
     else vi = intersect(vi, cModel->verseIndexes(sl.at(i)) );
  }
  cModel->iArray = vi;
  vListView->setModel( cModel->verseModel() );
};

void Concordance::onWLClicked(const QModelIndex &i){
   if (!cModel) vListView->setModel(0);
   else{
      int j = i.row();
      comboBox->disconnect(SIGNAL(editTextChanged(QString)));
      comboBox->setEditText( cModel->word(j) );
      connect(comboBox, SIGNAL(editTextChanged(QString)), this, SLOT(onEditTextChanged(QString)));
      cModel->words.clear();
      cModel->setIndexArray( j );
      vListView->setModel( cModel->verseModel() );
   }
};

void Concordance::onVLClicked(const QModelIndex &i){
   if (!cModel || !cModel->iArray) return;
   emit verseClicked( cModel->bible, cModel->iArray->at(i.row()) );
};

void Concordance::onEditTextChanged(const QString &t0){
   QString t = t0;
   QStringList sl = t0.split(" ");
   int i = sl.size()-1;
   if (i>-1) t = sl.at(i);
   i = cModel->indexOf(t);
   QModelIndex in = cModel->index(i,0,QModelIndex());
   if (t.toLower()==cModel->word(i).toLower()){
      wListView->setCurrentIndex(in);
      fillVList(sl);
   }
   else {
      wListView->setCurrentIndex(QModelIndex());
      wListView->scrollTo(in);
      vListView->setModel(0);
      cModel->iArray=0;
   }
};

// Изпълнява се при смяна на глобалния индекс на текущия стих и прави този стих да стане селектиран
// в списъка с намерени стихове
void Concordance::onGlobalIndexChanged(BibleWindow *bw){
    Q_UNUSED(bw);
   int ci = vListView->currentIndex().row(); // Номер на селектирания ред в списъка с намерени стихове
   int vi = cModel->bible->verseIndex();  // Локален индек на активния стих от Библията
   if (!vi) return; // Ако няма активен стих - изход
   int ni = -1;  // Номер на активния стих в списъка на намерените стихове
   if (cModel && cModel->iArray) ni = cModel->iArray->indexOf(vi);
   if (ci==ni) return; // Ако стихът не е друг - изход
   QModelIndex in = vListView->model()->index(ni,0,QModelIndex()); // Моделов индекс на текущия стих
   vListView->setCurrentIndex(in); // Селектиране
};

// Изпалнява се при щракване на бутона "в книгите..."
void Concordance::onPushButton(){
   if (!cModel) return;
   FilterDialog *fd = new FilterDialog(cModel,pushButton);
   fd->exec();
};

// Изпълнява се при затваряне на Библията
void Concordance::onBibleClosing(BibleWindow *bw){
    Q_UNUSED(bw);
   cModel = 0;
};

// Изпълнява се при приемане на сигнал, че е избрано "Копиране на препратките" от 
// контекстното меню на списъка с намерените стихове
void Concordance::onCopyList(int it){
   // Ако няма селектирани редове - изход
   if (!vListView->selectedIndexes().size()) return;
   // Диалог за промяна на разделителя
   if (vListView->selectedIndexes().size()>1){
     bool ok;
     QString nD = QInputDialog::getText(cModel->bible, 
                tr("Change if you like"),tr("Verse delimiter:"), QLineEdit::Normal, vDel, &ok);
     if (!ok) return;
     vDel = nD;
   }
   QString s = ""; // Стринг, който ще се изпрати в Clipboard
   // За всеки селектиран ред
   for(int i=0; i<vListView->selectedIndexes().size(); i++){
      int ri = vListView->selectedIndexes().at(i).row(); // Номер на селектирания ред
      int vi = -1;                             // Локален индекс на стиха, от селектирания ред
      if (!cModel || !cModel->iArray) return;
      vi = cModel->iArray->at(ri);
      QString r = ""; // Стринг, който се добавя за всеки стих
      switch (it){
      case 0: r = cModel->bible->reference(vi); break;
      case 1: r = cModel->bible->reference(vi,true) + " " + cModel->bible->verseText(vi); break;
      }
      if (s.size()) s.append(vDel);
      s.append(r);
   }
   QApplication::clipboard()->setText(s);
};

