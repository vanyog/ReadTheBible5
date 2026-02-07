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

#include <QTextCodec>

#include "language.h"

Language::Language(){

};

QString Language::chapter(const QString &ln){
   QByteArray ba = "Глава";
   if (ln=="en") ba = "Chapter";
   if (ln=="ro") ba = "Cpitolul";
   if (ln=="ru") ba = "Голова";
   return QString::fromUtf8(ba);
};

QString Language::psalm(const QString &ln){
   QByteArray ba = "Псалом";
   if (ln=="en") ba = "Psalm";
   if (ln=="ro") ba = "Psalmul";
   if (ln=="ru") ba = "Псалм";
   return QString::fromUtf8(ba);
};

QString Language::characters(const QString &ln){
   QByteArray ba = "АаБбВвГгДдЕеЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЬьЮюЯя";
   if (ln=="en") ba = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
   if (ln=="ro") ba = "AaĂăÂâBbCcDdEeFfGgHhIiÎîJjKkLlMmNnOoPpQqRrSsȘșTtȚțUuVvWwXxYyZz";
   if (ln=="ru") ba = "АаБбВвГгДдЕеЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя";
   return QString::fromUtf8(ba);
};

Language language_;

Language *languageObject(){
   return &language_;
};
