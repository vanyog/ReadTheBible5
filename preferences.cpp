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

#include "preferences.h"
#include "preferedColors.h"
#include "showMessage.h"

#include <QSettings>
#include <QFileDialog>
#include <QFontDialog>
#include <QNetworkProxy>

Preferences::Preferences( QWidget *parent, Qt::WindowFlags f ){
    Q_UNUSED(parent); Q_UNUSED(f);
   ui.setupUi(this);
   connect(ui.comboBox,  SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
   connect(ui.comboBox_2,SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
   connect(ui.spinBox,   SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
   connect(ui.pushButton,SIGNAL(clicked()), this, SLOT(onBrowse()));
   connect(ui.pushButton_2,SIGNAL(clicked()), this, SLOT(onFontChange()));
   connect(ui.pushButton_3,SIGNAL(clicked()), this, SLOT(onColorButtonPressed()));
   connect(this,SIGNAL(accepted()), this, SLOT(onAccepted()));
};

void Preferences::writeSettings(QSettings *s){
   s->setValue("preferences_Geometry",saveGeometry());
   s->setValue("preferences_tabIndex",ui.tabWidget->currentIndex());
   s->setValue("preferences_Footnotes",footnotes());
   s->setValue("preferences_References",references());
   s->setValue("preferences_MaxItemCount",ui.spinBox->value());
   s->setValue("preferences_CrossBgBible",ui.lineEdit->text());
   s->setValue("preferences_Font",font());
   s->setValue("preferences_QtDialog",ui.checkBox->checkState()==Qt::Checked);
   s->setValue("language",ui.comboBox_4->currentIndex());
   preferedColor()->writeSettings(s);
};

void Preferences::readSettings(QSettings *s){
   restoreGeometry(s->value("preferences_Geometry").toByteArray());
   ui.tabWidget->setCurrentIndex(s->value("preferences_tabIndex").toInt());
   ui.comboBox -> setCurrentIndex(s->value("preferences_Footnotes").toInt() );
   ui.comboBox_2->setCurrentIndex(s->value("preferences_References").toInt());
   int v = s->value("preferences_MaxItemCount").toInt();
   if (v) ui.spinBox  -> setValue(v);
   ui.lineEdit -> setText(s->value("preferences_CrossBgBible").toString());
   QFont f = s->value("preferences_Font").value<QFont>();
   qApp->setFont(f);
   ui.label_6->setText(f.family() + " " + QString::number(f.pointSize()) );
   if (s->value("preferences_QtDialog").toBool()) ui.checkBox->setCheckState(Qt::Checked);
   else ui.checkBox->setCheckState(Qt::Unchecked);
   ui.comboBox_4->setCurrentIndex(s->value("language").toInt());
   preferedColor()->readSettings(s);
};

int Preferences::footnotes(){
   return ui.comboBox->currentIndex();
};

int Preferences::references(){
   return ui.comboBox_2->currentIndex();
};

QString Preferences::CrossBgBiblePath(){
   return ui.lineEdit->text();
};

void Preferences::showEvent(QShowEvent *event){
    Q_UNUSED(event);
   ui.label_6->setText(font().family() + " " + QString::number(font().pointSize()) );
   proxyType = ui.comboBox_3->currentIndex();
   langIndex = ui.comboBox_4->currentIndex();
};

void Preferences::onCurrentIndexChanged(int i){
   Q_UNUSED(i);
   emit optionChanged();
};

void Preferences::onValueChanged(int i){
   emit maxItemsChanged(i);
};

void Preferences::onBrowse(){
   QString fn = QFileDialog::getOpenFileName( this, tr("CrossBgBible's path"), ui.lineEdit->text() );
   if (fn.size()) ui.lineEdit->setText( fn );
};

void Preferences::onFontChange(){
   bool ok;
    QFontDialog::FontDialogOptions opt = QFontDialog::FontDialogOptions();
   if (ui.checkBox->checkState()==Qt::Checked) opt = QFontDialog::DontUseNativeDialog;
   QFont f = QFontDialog::getFont(&ok,font(),this,tr("Change Font"), opt);
   if (ok){
      ui.label_6->setText(f.family() + " " + QString::number(f.pointSize()) );
      qApp->setFont(f);
   }
};

void Preferences::onColorButtonPressed(){
   preferedColor()->exec();
};

void Preferences::onAccepted(){
   int i=ui.comboBox_3->currentIndex();
   bool ok;
   int port=ui.lineEdit_3->text().toInt(&ok,16);
   Q_UNUSED(port);
   if (((proxyType!=i)
       ||ui.lineEdit_2->isModified()
       ||ui.lineEdit_3->isModified()
       ||ui.lineEdit_4->isModified()
       ||ui.lineEdit_5->isModified()
       )
       && ok
      )
   {
     setProxy();
     showMessage(tr("Application proxy has been set."));
   };
   if (langIndex!=ui.comboBox_4->currentIndex())
      showMessage(tr("Language change will take effect next time you start the program."));
};

void Preferences::setProxy(){
   QNetworkProxy proxy;
   proxy.setType(QNetworkProxy::ProxyType(ui.comboBox_3->currentIndex()));
   proxy.setHostName(ui.lineEdit_2->text());
   proxy.setPort(port);
   proxy.setUser(ui.lineEdit_4->text());
   proxy.setPassword(ui.lineEdit_5->text());
   QNetworkProxy::setApplicationProxy(proxy);
};

//-----------Functions--------------

Preferences *_preferences = 0;
PreferedColor *_preferedColor = 0;

void createPreferences(QWidget *parent){
   _preferences = new Preferences(parent);
   _preferedColor = new PreferedColor(_preferences);
};

Preferences *preferences(){
   return _preferences;
};

PreferedColor *preferedColor(){
   return _preferedColor;
};
