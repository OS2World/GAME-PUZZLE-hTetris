# Borland C++ is required to compile this program.
# Correct LIBPATH and INCLUDEPATH in this file 
# and type at command prompt to compile:
# make

# hTetris       v1.1b4 (C)  3-Jan..29 May-1999, //SUE.AtH@hMoscow
#
# TETRIS GAME!!!
# Created by Software Underground Empire Hackers' Group (SUE),
#         in Civil Hackers' School Moscow Center (hMoscow),
#         Moscow ex-Pioneer Palace.
# Contacts: hscool@netclub.ru
#           Ilya_Vasilyev, 2:5020/287@fidonet.org
# Translated to C under OS/2 by dsp//SUE
#                              (Dmitriy Ponomarenko, 2:5020/1644.16@fidonet.org)

#   Copyright (C) 1999, SUE Group
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#=============================================================

.AUTODEPEND

.PATH.obj = .\

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +HTETRIS.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = C:\BCOS2\LIB
INCLUDEPATH = C:\BCOS2\INCLUDE


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 htetris.def \
 htetris.obj \
 platdep.obj

#=============================================================
#		Explicit Rules
#=============================================================
.\htetris.exe: htetris.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /wmsk /wdpl /Toe /ap /L$(LIBPATH) @&&|
$(LIBPATH)\C02.OBJ+
.\htetris.obj+
.\platdep.obj
.\htetris,.\htetris
$(LIBPATH)\C2MT.LIB+
$(LIBPATH)\OS2.LIB
htetris.def
|


#=============================================================
#		Individual File Dependencies
#=============================================================
htetris.obj: htetris.cfg htetris.c

platdep.obj: htetris.cfg platdep.c

#=============================================================
#		Compiler Configuration File
#=============================================================
htetris.cfg: makefile
  copy &&|
-xf
-xp
-Oi
-Oz
-Ob
-Oe
-Oc
-L$(LIBPATH)
-I$(INCLUDEPATH)
-n.
-H=.\htetris.CSM
-vi-
-sm
-k-
-O
-Ot
-C
-a
| htetris.cfg


