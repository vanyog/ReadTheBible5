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

#include <QFileDialog>
#include <QSettings>

#include "exportDialog.h"
#include "showMessage.h"

ExportDialog::ExportDialog( QWidget * parent, Qt::WindowFlags f ):
  QDialog( parent, f )
{
    ui.setupUi(this);
    QSettings s("VanyoG", "CD Bible 5");
    QString d = s.value("eport_dir").toString();
    if(d.size()==0) {
        QDir dir(QCoreApplication::applicationDirPath());
#ifdef Q_OS_MAC
        dir.cdUp(); dir.cdUp(); dir.cdUp();
#endif
        d = dir.absolutePath();
    }
    ui.label_2->setText(d);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onBrowse()));
};

ExportDialog::Export ExportDialog::whatToExport(){
   if (ui.radioButton_2->isChecked()) return Book;
   if (ui.radioButton_3->isChecked()) return Bible;
   return Chapter;
};

QString ExportDialog::toString(){
   if (ui.radioButton_2->isChecked()) return "Book";
   if (ui.radioButton_3->isChecked()) return "Bible";
   return "Chapter";
};

void ExportDialog::setFileName(const QString &fn){
    ui.lineEdit->setText(fn);
};

QString ExportDialog::fileName(){
    return ui.label_2->text() + "/" + ui.lineEdit->text();
};

void ExportDialog::onBrowse(){
    QString fn = QFileDialog::getSaveFileName(this, tr("Save to"), fileName() );
    QFileInfo i(fn);
    ui.label_2->setText(i.path());
   if (fn.size()) setFileName(i.fileName());
};

