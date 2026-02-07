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

#include "history.h"
#include "showMessage.h"

History::History(){
  c_i=-1; m_i=-1;
};

void History::push(int i){
   c_i++;
   if (c_i<list.size()) list[c_i] = i;
   else list.append(i);
   m_i=c_i;
};

int History::back(){
   int r = -1;
   if (c_i>0){
      c_i--;
      r = list.value(c_i);
   }
   return r;
};

int History::forward(){
   int r = -1;
   if (c_i<m_i){
      c_i++;
      r = list.value(c_i);
   }
   return r;

};
