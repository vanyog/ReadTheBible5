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

#include <QByteArray>
#include <QString>

#include "showMessage.h"

QString myDecode(const QByteArray &tx, const QString &cd){
    QString rz = "";
    if (cd == "cp1251"){ //countedMessage("aaa");
        for(char c : tx){
            int code = static_cast<unsigned char>(c);
            if(code>191 && code<256) rz += QChar(code + 848);
            else{
                switch (code) {
                case 144: rz += QChar(1116); break;
                case 154: rz += QChar(1113); break;
                case 156: rz += QChar(1114); break;
                case 158: rz += QChar(1115); break;
                case 163: rz += QChar(1032); break;
                case 182: rz += QChar(182); break;
                case 184: rz += QChar(1105); break;
                case 188: rz += QChar(1112); break;
                default:
                        if (code>127){ rz += QChar(c); countedMessage(rz + QString::number(code)); }
                        else rz += QChar(c);
                }
            }
        }
    }
    else if(cd == "UTF-8")
        rz = QString(tx);
    return rz;
}
