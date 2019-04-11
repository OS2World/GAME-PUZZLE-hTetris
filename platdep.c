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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys\timeb.h>
#include <math.h>

#ifdef __OS2__
#  define INCL_DOSPROCESS
#  define INCL_DOSSEMAPHORES
#  include <os2.h>
#endif
#ifdef __MT__
#  include <process.h>
#  include <mem.h>
#endif
#include "platdep.h"

bool flgMusic=true, flgPlaying=false;
static unsigned wOldCursorSize;

#if defined(__TURBOC__)
char KeyGet(bool flgWait)
{ char c;
  if(!flgWait && !kbhit()) return 0;
  if(!(c=getch()))
    switch(c=getch())
    { case 72: if(flgMusic && !flgPlaying) Sound(300, 0.05); return KG_KEYUP;
      case 75: if(flgMusic && !flgPlaying) Sound(300, 0.05); return KG_KEYLEFT;
      case 77: if(flgMusic && !flgPlaying) Sound(300, 0.05); return KG_KEYRIGHT;
      case 80: if(flgMusic && !flgPlaying) Sound(300, 0.05); return KG_KEYDOWN;
      case 68: if(flgMusic && !flgPlaying) Sound(300, 0.05); return KG_KEYF10;
      case 61: if(flgMusic && !flgPlaying) Sound(300, 0.05); return KG_KEYF3;
    }
  if(flgMusic && !flgPlaying) Sound(300, 0.05);
  return c;
}
#endif

#if defined(__TURBOC__)
int Random(int iMax)
{ return random(iMax);
}
#endif

double Timer(void)
{ struct timeb buf;
  ftime(&buf);
  return (double)buf.time+(double)buf.millitm/(double)1000;
}

#if defined(__TURBOC__)
double Rnd(void)
{ return (double)rand()/(double)RAND_MAX;
}
#endif

char *sTime(void)
{ time_t t;
  struct tm *tp;
  char *sRet=malloc(6);
  if(!sRet)
  { fprintf(stderr, "Not enough memory\n"); abort(); }
  time(&t);
  tp=localtime(&t);
  sprintf(sRet, "%2d:%02d", tp->tm_hour, tp->tm_min);
  return sRet;
}

#if defined(__TURBOC__)
void KbdBufClean(void)
{ bool flgOldMusic=flgMusic;
  flgMusic=false;
  while(kbhit()) KeyGet(true);
  flgMusic=flgOldMusic;
}
#endif


#if defined(__OS2__) || defined(__MSDOS__)
  void MusicSet(bool f)
  { flgMusic=f;
    gotoxy(MUSIC_X, MUSIC_Y); textcolor(6); textbackground(0);
    if(flgMusic) cputs("Music ON"); else cputs(" (mute) ");
  }
#else
  void MusicSet(bool f) {}
#endif

#if defined(__OS2__)
  void Beep(int iFreq, int iMSec)
  { DosBeep(iFreq, iMSec); }
#elif defined(__MSDOS__)
  void Beep(int iFreq, int iMSec)
  { sound(iFreq); Wait(iMSec); nosound(); }
#else
  void Beep(int iFreq, int iMSec)
  { }
#endif

#if defined(__OS2__) || defined(__MSDOS__)
  void Sound(int iFreq, double fDur)
  { Beep(iFreq, (unsigned long)(fDur*(double)1000/(double)18.2));
  }
#else
  void Sound(int iFreq, double fDur) {}
#endif

#if defined(__BORLANDC__)
  void SetCursorType(int t)
  { _setcursortype(t);
  }
#elif defined(__MSDOS__)
  void SetCursorType(int t)
  { union REGS outr, inr;
    outr.h.ah=0x01;
    switch(t)
    { case _NOCURSOR:
	outr.h.ch=0x20;
	break;
      case _SOLIDCURSOR:
	outr.h.ch=0x00;
	outr.h.cl=31;
	break;
    }
    int86(0x10, &outr, &inr);
  }
#endif

#if defined(__MSDOS__)
   void CursorSave(void)
   { union REGS outr, inr;
     outr.h.ah=0x01;
     int86(0x10, &outr, &inr);
     wOldCursorSize=inr.x.cx;
   }
   void CursorRestore(void)
   { union REGS outr, inr;
     outr.x.cx=wOldCursorSize;
     outr.x.ax=0x0001;
     int86(0x10, &outr, &inr);
   }
#else
  void CursorSave(void)
  {
  }
  void CursorRestore(void)
  {
# ifdef _NORMALCURSOR
  _setcursortype(_NORMALCURSOR);
# endif
  }
#endif

void Wait(int msec)
#if defined(__OS2__)
{ if(msec>=0)
    DosSleep(msec);
}
#else
{ double t=Timer();
  if(msec>=0)
    while(Timer()-t<(double)msec/1000.0);
}
#endif

#if defined(__MT__) && defined(__OS2__)
#if defined(__TURBOC__)
int BeginThread(void (*fn)(void *), size_t stack, void *args)
{ return _beginthread(fn, stack, args);
}
#endif
void KillThread(int tid)
{ DosKillThread(tid);
}
void CritSecEnter(void)
{ DosEnterCritSec();
}
void CritSecExit(void)
{ DosExitCritSec();
}
void SuspendThread(int tid)
{ DosSuspendThread(tid);
}
void WaitThread(int *tid)
{ DosWaitThread(tid, DCWW_WAIT);
}
#if defined(__TURBOC__)
void EndThread(void)
{ _endthread();
}
#endif
unsigned long CreateEventSem(char *sName, bool flgShared, bool flgPosted)
{ HEV hEventSem=NULLHANDLE;
  DosCreateEventSem(sName, &hEventSem, flgShared ? DC_SEM_SHARED : 0, flgPosted);
  return hEventSem;
}
void PostEventSem(unsigned long hEv)
{ DosPostEventSem(hEv);
}
unsigned long ResetEventSem(unsigned long hEv)
{ unsigned long ulPostCount;
  DosResetEventSem(hEv, &ulPostCount);
  return ulPostCount;
}
bool WaitEventSem(unsigned long hEv, long lTimeout, bool flgIndefinite)
{ if(!flgIndefinite && lTimeout<=0) lTimeout=SEM_IMMEDIATE_RETURN;
  if(flgIndefinite) lTimeout=SEM_INDEFINITE_WAIT;
  return DosWaitEventSem(hEv, lTimeout)?false:true;
}
void CloseEventSem(unsigned long hEv)
{ DosCloseEventSem(hEv);
}
#endif
