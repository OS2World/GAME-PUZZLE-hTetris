/*
  Borland C++ is required to compile this program.
  Correct LIBPATH and INCLUDEPATH and type at command prompt to compile:
  make

  hTetris       v1.1b4 (C)  3-Jan..29 May-1999, //SUE.AtH@hMoscow
 
  TETRIS GAME!!!
  Created by Software Underground Empire Hackers' Group (SUE),
          in Civil Hackers' School Moscow Center (hMoscow),
          Moscow ex-Pioneer Palace.
  Contacts: hscool@netclub.ru
            Ilya_Vasilyev, 2:5020/287@fidonet.org
  Translated to C under OS/2 by dsp//SUE
                               (Dmitriy Ponomarenko, 2:5020/1644.16@fidonet.org)

    Copyright (C) 1999, SUE Group
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/* Platform-dependent functions */
#ifndef PLATDEP_H
#define PLATDEP_H

#ifdef __TURBOC__
#  include <conio.h>
#  include <dos.h>
#  ifndef __cplusplus
     enum bool { true=1, false=0 };
     typedef int bool;
#  endif
#else
#  error Unsupported platform.
#endif

#define KG_KEYUP	1
#define KG_KEYLEFT	2
#define KG_KEYDOWN	3
#define KG_KEYRIGHT	4
#define KG_KEYF10	5
#define KG_KEYF3	6

#define STAKAN_X	26
#define STAKAN_Y	2
#define	STAKAN_WIDTH	10
#define STAKAN_HEIGHT	22
#define MUSIC_X		(STAKAN_X+18+STAKAN_WIDTH*2)
#define MUSIC_Y		(STAKAN_Y+4)

#ifndef _NOCURSOR
#  define _NOCURSOR	0
#endif

#ifndef _SOLIDCURSOR
#  define _SOLIDCURSOR	1
#endif

#ifdef __MT__
#  ifndef __OS2__
#    error Unsupported multithreaded platform.
#  endif
#endif

extern bool flgMusic;
extern bool flgPlaying;

char KeyGet(bool flgWait);
int Random(int iMax);
double Timer(void);
double Rnd(void);
char *sTime(void);
void KbdBufClean(void);
void MusicSet(bool f);
void Sound(int iFreq, double fDur);
void Beep(int iFreq, int iMSec);
void SetCursorType(int t);
void CursorSave(void);
void CursorRestore(void);
void Wait(int msec);
#ifdef	__MT__
#  ifdef	__OS2__
#    define	THREAD_MAIN	1
#  endif
int BeginThread(void (*fn)(void *), size_t stack, void *args);
void KillThread(int tid);
void CritSecEnter(void);
void CritSecExit(void);
void SuspendThread(int tid);
void WaitThread(int *tid);
void EndThread(void);
unsigned long CreateEventSem(char *sName, bool flgShared, bool flgPosted);
void PostEventSem(unsigned long hEv);
unsigned long ResetEventSem(unsigned long hEv);
bool WaitEventSem(unsigned long hEv, long lTimeout, bool flgIndefinite);
void CloseEventSem(unsigned long hEv);
#endif

#endif
