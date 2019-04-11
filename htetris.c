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
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "platdep.h"

#define	FIG_MAX		18
#define EX_MAX		5


#define SCORE_X		(STAKAN_X+STAKAN_WIDTH*2+11)
#define SCORE_Y		9
#define	LEVEL_X		(STAKAN_X+STAKAN_WIDTH*2+11)
#define LEVEL_Y		10
#define	LINES_X		(STAKAN_X+STAKAN_WIDTH*2+11)
#define LINES_Y		11
#define LVLTXT_X	1
#define LVLTXT_Y	1
#define NXT_Y		4
#define NXT_X		(STAKAN_X+STAKAN_WIDTH*2+7)

#define LEVEL_NAME_MAX		20
#define LEVEL_NAME_LEN_MAX	33

void AllInit(void);
void BackDraw(void);
void CellSet(int x, int y, char c);
void ExBomb(void);
void ExBurner(void);
void ExHBuilder(void);
void ExPress(void);
void ExUnknown(void);
void ExVBuilder(void);
char *FieldInput(int x, int y, const char *sDefault, size_t Size);
void FigCheck(int dx, int dy);
void FigDraw(void);
void FigMove(int dx, int dy);
void FigNext(void);
void FigRotate(void);
void FigSave(void);
void FigUnDraw(void);
void GameOver(void);
void GamePause(void);
void GameStart(void);
void InitMusic(void);
void iSwap(int *i1, int *i2);
void LevelInc(void);
void LevelPrint(void);
void LinesCheck(void);
void LinesDraw(int ys, int ye, bool flgDelete);
void LinesInit(void);
char *LTrim(char *s);
void NxtDraw(void);
void NxtFrame(bool flgParam);
void NxtSwap(void);
void Play(const char *PlayString);
void PlayThread(void *PlayString);
char *RTrim(char *s);
void ScoreAdd(int i);
void sSwap(char *s1, char *s2);
void TableRead(bool flgKeyWait);
void TableRemove(int iStart, int iNumber, int iDirection);
void TableShow(bool flgKeyWait);
void TableWrite(void);
char *UpperStr(char *s);
char *UpperStrN(const char *s);

#ifdef __MT__
  void KbdPollThread(void *args);
#endif

/*  0123  0123  0123  0123  0123
   0.... 0.X.. 0.... 0.... 0....
   1.... 1.X.. 1X... 1.XX. 1.XX.
   2XXXX 2.X.. 2XX.. 2XX.. 2.XX.
   3.... 3.X.. 3.X.. 3.... 3....*/
int iFigsX[FIG_MAX+1][4]={ { 0,1,2,3 },   /* 0 */
			   { 1,1,1,1 },   /* 1 */
			   { 0,0,1,1 },   /* 2 */
			   { 0,1,1,2 },   /* 3 */
			   { 1,1,2,2 },   /* 4 */
/*  0123  0123  0123  0123  0123
   0.... 0.... 0..X. 0.... 0....
   1.XX. 1.X.. 1..X. 1XXX. 1XX..
   2.X.. 2.XXX 2.XX. 2..X. 2.XX.
   3.X.. 3.... 3.... 3.... 3....*/
			   { 1,1,1,2 },   /* 5 */
			   { 1,1,2,3 },   /* 6 */
			   { 1,2,2,2 },   /* 7 */
			   { 0,1,2,2 },   /* 8 */
			   { 0,1,1,2 },   /* 9 */
/*  0123  0123  0123  0123  0123
   0.... 0.... 0.... 0.... 0....
   1.X.. 1.X.. 1.X.. 1.... 1.X..
   2XX.. 2XXX. 2XX.. 2XXX. 2.XX.
   3X... 3.... 3.X.. 3.X.. 3.X..*/
			   { 0,0,1,1 },   /* 10 */
			   { 0,1,1,2 },   /* 11 */
			   { 0,1,1,1 },   /* 12 */
			   { 0,1,1,2 },   /* 13 */
			   { 1,1,1,2 },   /* 14 */
/*  0123  0123  0123  0123
   0.... 0.... 0.X.. 0....
   1.XX. 1.XXX 1.X.. 1..X.
   2..X. 2.X.. 2.XX. 2XXX.
   3..X. 3.... 3.... 3....*/
			   { 1,2,2,2 },   /* 15 */
			   { 1,1,2,3 },   /* 16 */
			   { 1,1,1,2 },   /* 17 */
			   { 0,1,2,2 } }; /* 18=FIG_MAX */

/*  0123  0123  0123  0123  0123
   0.... 0.X.. 0.... 0.... 0....
   1.... 1.X.. 1X... 1.XX. 1.XX.
   2XXXX 2.X.. 2XX.. 2XX.. 2.XX.
   3.... 3.X.. 3.X.. 3.... 3....*/
int iFigsY[FIG_MAX+1][4]={ { 2,2,2,2 },   /* 0 */
			   { 0,1,2,3 },   /* 1 */
			   { 1,2,2,3 },   /* 2 */
			   { 2,1,2,1 },   /* 3 */
			   { 1,2,1,2 },   /* 4 */
/*  0123  0123  0123  0123  0123
   0.... 0.... 0..X. 0.... 0....
   1.XX. 1.X.. 1..X. 1XXX. 1XX..
   2.X.. 2.XXX 2.XX. 2..X. 2.XX.
   3.X.. 3.... 3.... 3.... 3....*/
			   { 1,2,3,1 },   /* 5 */
			   { 1,2,2,2 },   /* 6 */
			   { 2,0,1,2 },   /* 7 */
			   { 1,1,1,2 },   /* 8 */
			   { 1,1,2,2 },   /* 9 */
/*  0123  0123  0123  0123  0123
   0.... 0.... 0.... 0.... 0....
   1.X.. 1.X.. 1.X.. 1.... 1.X..
   2XX.. 2XXX. 2XX.. 2XXX. 2.XX.
   3X... 3.... 3.X.. 3.X.. 3.X..*/
			   { 2,3,1,2 },   /* 10 */
			   { 2,1,2,2 },   /* 11 */
			   { 2,1,2,3 },   /* 12 */
			   { 2,2,3,2 },   /* 13 */
			   { 1,2,3,2 },   /* 14 */
/*  0123  0123  0123  0123
   0.... 0.... 0.X.. 0....
   1.XX. 1.XXX 1.X.. 1..X.
   2..X. 2.X.. 2.XX. 2XXX.
   3..X. 3.... 3.... 3....*/
			   { 1,1,2,3 },   /* 15 */
			   { 1,2,1,1 },   /* 16 */
			   { 0,1,2,2 },   /* 17 */
			   { 2,2,1,2 } }; /* 18=FIG_MAX */

int iFigsRot[FIG_MAX+1]={ 1,0,3,2,4,6,7,8,5,10,9,12,13,14,11,16,17,18,15 };

int iExFg[EX_MAX+1]={ 12, 4, 12, 6, 6, 6 };
int iExBg[EX_MAX+1]={ 0, 0, 0, 0, 0, 4 };
char sExChar[EX_MAX+1][3]={ "<>", "‹‹", "XX", "", "", "®?" };
char sNames[9][100]; long lScores[9]; int iLevels[9];
int iPlayersNum;
char sPlayerName[18]="(unknown)";
int iMyLevelMax=1;
int iStakan[STAKAN_WIDTH+4][STAKAN_HEIGHT+2];
int iStakanBg[STAKAN_WIDTH][STAKAN_HEIGHT+1];
int iStakanFg[STAKAN_WIDTH][STAKAN_HEIGHT+1];
float fGravity=0;
bool flgCollide=false;
bool flgGameOver=false;
int iLevel=1;
int iNxtBg=0, iNxtFg=0, iFigBg=0, iFigFg=0;
int iFigX=0, iFigY=0, iFigN=0, iNxtN=0;
char sFigChar[3]={ '\0', '\0', '\0' };
char sNxtChar[3]={ '\0', '\0', '\0' };
long lScore=0; int iLines=0;
long lOldScore; int iOldLines;
bool flgLevel=false, flgClassic=false, flgNext=true;

#ifdef __MT__
  char cChar='\0';
  bool flgKbdPoll=false;
  bool flgKeyPress=false;
  int PlayTID=0;
  int KbdPollTID=0;
  unsigned long hKeyPressSem;
#endif

char *sBuffer=NULL, *sTemp=NULL;
char sLevelNames[LEVEL_NAME_MAX][LEVEL_NAME_LEN_MAX+1]=
{ "A beginning...",	/* 1 */
  "Color Parade",    	/* 2 */
  "...blinkenlights.",	/* 3 */
  "Bombing Run",    	/* 4 */
  "Under Pressure",	/* 5 */
  "Stone Burner",    	/* 6 */
  "Stakan Wide Web",	/* 7 */
  "Manna",	    	/* 8 */
  "The Infinite Improbability Drive", /* 9 */
  "Mercury","Venus", "Earth", "Mars","Faeton",
  "Jupiter","Saturn","Neptun","Uran","Pluto",
  "Terra Incognita"	/* 20 */
};
struct text_info oldtextinfo;

int main(void)
{ char *st;
  double t=0;
  AllInit();
  while(1)
  { KbdBufClean();
    GameStart();
    flgGameOver=false; lScore=0; iLines=0;
    iOldLines=iLines; lOldScore=lScore;
    BackDraw();
    LevelPrint();
    MusicSet(flgMusic);
    if(flgNext) NxtFrame(flgNext);
    LinesInit();
    do
    { textcolor(0); textbackground(3); gotoxy(75, 1);
      cputs(st=sTime()); free(st);
      fGravity=(double)0.5/(double)iLevel; FigSave(); LinesCheck();
      if(flgLevel)
      { flgLevel=false; LevelInc(); LinesInit(); LinesDraw(1, STAKAN_HEIGHT, 0);
	iOldLines=iLines; lOldScore=lScore;
	FigNext();
	LevelPrint();
      }
      flgCollide=false; FigNext(); FigDraw(); FigCheck(0, 0);
      if(flgCollide) break;
#if defined(__MT__)
      ResetEventSem(hKeyPressSem);
      t=Timer();
#else
      KbdBufClean();
#endif
      while(1)
      {
#if defined(__MT__)
	if(flgKeyPress)
	  ResetEventSem(hKeyPressSem);
	flgKeyPress=false;
	if(!flgKbdPoll && fGravity)
	{ if(KbdPollTID && KbdPollTID!=(flgPlaying?PlayTID:-1))
	    KillThread(KbdPollTID);
	  KbdPollTID=BeginThread(KbdPollThread, 8192, NULL);
	}
	if(fGravity)
	  flgKeyPress=WaitEventSem(hKeyPressSem, (fGravity-(Timer()-t))*1000, false);
	if(fGravity && flgKeyPress)
	  switch(toupper(cChar))
#else
	if(fGravity)
	  switch(toupper(KeyGet(false)))
#endif
	  { case '2':
	    case KG_KEYDOWN:
	      FigMove(0, 1);
	      break;
	    case '4':
	    case KG_KEYLEFT:
	      FigMove(-1, 0);
	      break;
	    case '5':
	    case '8':
	    case KG_KEYUP:
	      FigRotate();
	      break;
	    case '6':
	    case KG_KEYRIGHT:
	      FigMove(1, 0);
	      break;
	    case 27:
	      flgGameOver=true;
	      break;
	    case ' ':
	      fGravity=0;
	      break;
	    case 'N':
	      NxtFrame(flgNext=!flgNext);
	      if(flgNext && iNxtN>=0) NxtDraw();
	      break;
	    case 'S':
	      if(flgNext) NxtSwap();
	      break;
	    case 'P':
	      GamePause();
	      break;
	    case 'M':
	      MusicSet(!flgMusic);
	      break;
	  }
	if(flgGameOver)
#ifdef __MT__
	{ ResetEventSem(hKeyPressSem);
#endif
	  break;
#ifdef __MT__
	}
#endif
	if(Timer()-t>fGravity)
	{ t=Timer(); flgCollide=false; FigMove(0, 1);
	  if(flgCollide) break;
	  if(!fGravity)
	    if(iFigFg>15) ScoreAdd(5);
	    else ScoreAdd(1);
	}
      }
    } while(!flgGameOver);
    GameOver();
  }
  return 0;
}

void AllInit(void)
{ gettextinfo(&oldtextinfo);
  textmode(C80);
  clrscr();
  CursorSave();
  SetCursorType(_NOCURSOR);
  InitMusic();
#ifdef __MT__
  hKeyPressSem=CreateEventSem(NULL, false, false);
#endif
}

void BackDraw(void)
{ int bby, i;
  textcolor(7); textbackground(0); clrscr();
  textcolor(15);
  for(bby=STAKAN_Y; bby<=STAKAN_Y+STAKAN_HEIGHT; ++bby)
  { gotoxy(STAKAN_X+1, bby); putch('≥');
    gotoxy(STAKAN_X+2+STAKAN_WIDTH*2, bby); putch('≥');
  }
  gotoxy(STAKAN_X+1, bby); putch('¿');
  for(i=STAKAN_X+2; i<=STAKAN_X+1+STAKAN_WIDTH*2; ++i)
  { gotoxy(i, bby); putch('ƒ'); }
  gotoxy(i, bby); putch('Ÿ');
  textcolor(4);
  gotoxy(5, 4); cputs("   ‹€€€€€€€€‹");
  gotoxy(5, 5); cputs(" ‹€€ﬂ      ﬂ€€‹");
  textcolor(2);
  gotoxy(5, 6); cputs("€€›          ﬁ€€");
  gotoxy(5, 7); cputs("€€€€€€€€€€€€€€€€");
  textcolor(1);
  gotoxy(5, 8); cputs("€€›    ‹€€ﬂ  ﬁ€€");
  gotoxy(5, 9); cputs("€€›  ‹€€ﬂ    ﬁ€€");
  textcolor(6);
  gotoxy(6, 11);
  if(flgClassic) cputs("Classic tetris"); else cputs("    hTETRIS");
  gotoxy(7, 12); cputs("Version 1.1b4");
  textcolor(0); textbackground(3);
  gotoxy(1, 14); cputs(" …ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª ");
  gotoxy(1, 15); cputs(" ∫        Help       ∫ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 16); cputs(" «ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ∂ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 17); cputs(" ∫       Move        ∫ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 18); cputs(" ∫       Music On/Off∫ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 19); cputs(" ∫       Swap next   ∫ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 20); cputs(" ∫       Drop        ∫ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 21); cputs(" ∫       End game    ∫ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 22); cputs(" ∫       Pause       ∫ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 23); cputs(" ∫       Next On/Off ∫ "); textcolor(7); textbackground(0); cputs("∞∞");
  textcolor(0); textbackground(3);
  gotoxy(1, 24); cputs(" »ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº "); textcolor(7); textbackground(0); cputs("∞∞");
  gotoxy(3, 25); cputs("∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞");
  textcolor(15); textbackground(3);
  gotoxy(3, 17); cputs("Arrows");
  gotoxy(3, 18); putch('M');
  gotoxy(3, 19); putch('S');
  gotoxy(3, 20); cputs("Space");
  gotoxy(3, 21); cputs("Esc");
  gotoxy(3, 22); putch('P');
  gotoxy(3, 23); putch('N');
  textcolor(14); textbackground(9);
  gotoxy(SCORE_X-7, SCORE_Y); cputs(" Score:     0 ");
  gotoxy(LEVEL_X-7, LEVEL_Y); cprintf(" Level:%6d ", iLevel);
  gotoxy(LINES_X-7, LINES_Y); cputs(" Lines:     0 ");
  textcolor(6); textbackground(0);
  gotoxy(STAKAN_X+10+STAKAN_WIDTH*2, STAKAN_Y+11); cputs("CREATED IN MAY-99 BY:");
  textcolor(4); textbackground(0);
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+12); cputs("€ﬂﬂ     …Õ ∫");
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+13); cputs("ﬂﬂ€ …Õª ŒÕ ŒÕ »ª∫…º …Õπ ÀÕª …Õª");
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+14); cputs("ﬂﬂﬂ »Õº ∫  »Õ  » º  »Õ» ∫   ÃÕº");
  gotoxy(STAKAN_X+32+STAKAN_WIDTH*2, STAKAN_Y+15);                            cputs("»Õº");
  textcolor(2);
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+15); cputs("€ €     ∫");
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+16); cputs("€ €ÃÕª…Õπ…ÕªÀÕª             ∫");
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+17); cputs("ﬂﬂﬂ∫ ∫»ÕºÃÕº∫");
  gotoxy(STAKAN_X+13+STAKAN_WIDTH*2, STAKAN_Y+18);         cputs("»Õº");
  gotoxy(STAKAN_X+17+STAKAN_WIDTH*2, STAKAN_Y+17); cputs("…ÕªÀÕª ∫ ∫ÃÕª…Õπ");
  gotoxy(STAKAN_X+17+STAKAN_WIDTH*2, STAKAN_Y+18); cputs("»Õπ∫…Õª»Õº∫ ∫»Õº");
  gotoxy(STAKAN_X+17+STAKAN_WIDTH*2, STAKAN_Y+19); cputs("»Õº »Õº");
  textcolor(1);
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+19); cputs("€ﬂﬂ");
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+20); cputs("€ﬂﬂ …ÕÀÕª …Õª =  ÀÕª …Õª");
  gotoxy(STAKAN_X+4+STAKAN_WIDTH*2, STAKAN_Y+21); cputs("ﬂﬂﬂ ∫ ∫ ∫ ÃÕº ∫  ∫   ÃÕº");
  gotoxy(STAKAN_X+14+STAKAN_WIDTH*2, STAKAN_Y+22);          cputs("∫   »Õ     »Õº");
  textcolor(7);
  textcolor(6);
  gotoxy(STAKAN_X+18+STAKAN_WIDTH*2, STAKAN_Y+1); cputs("Player:");
  textcolor(14);
  gotoxy(STAKAN_X+18+STAKAN_WIDTH*2, STAKAN_Y+2); cputs(sPlayerName);
}

void CellSet(int x, int y, char c)
{ iStakanFg[x-1][y]=iFigFg; iStakanBg[x-1][y]=iFigBg; iStakan[x+1][y]=(int)c;
}

void ExBomb(void)
{ static const int iExplode[33]={ 16,
				  0, 0,  -1, 0,   0,-1,   1, 0,   0, 1,   0, 0,
				  -1,1,  -1,-1,   1,-1,   1, 1,   0, 0,
				  0, 2,  -2, 0,   0,-2,   2, 0,   0, 0 };
  int i, j, dx, dy, x, y, d=0, n;
  double t=Timer();
  for(j=0; j<2; ++j)
  { n=iExplode[d++];
    for(i=1; i<=n; ++i)
    { dx=iExplode[d++]; dy=iExplode[d++];
      x=iFigX+dx; y=iFigY+dy;
      if(!dx && !dy && flgMusic) Play("mfl64o1gcdco3");
      if(x<0 || y<0) break;
      if(iStakan[x+1][y]==2) break;
      if(iStakan[x+1][y]) ScoreAdd(5);
      gotoxy(STAKAN_X+x*2, STAKAN_Y+y);
      textbackground(0);
      if(j)
      { textcolor(7);
	iStakan[x+1][y]=iStakanBg[x-1][y]=iStakanFg[x-1][y]=0;
	cputs("  ");
      }
      else
      { textcolor(4); cputs("**");
      }
      if(!dx && !dy && !j)
      {	Wait(10-(Timer()-t)*1000); t=Timer(); }
    }
    d=0;
  }
}

void ExBurner(void)
{ double t=Timer();
  int x=STAKAN_X+iFigX*2, y, i;
  for(y=iFigY+1; y<=STAKAN_HEIGHT; ++y)
  { iStakanFg[iFigX-1][y]=iStakanBg[iFigX-1][y]=0;
    if(iStakan[iFigX+1][y]) { iStakan[iFigX+1][y]=0; ScoreAdd(5); }
    gotoxy(x, STAKAN_Y+y-1); textcolor(0); textbackground(0); cputs("  ");
    gotoxy(x, STAKAN_Y+y); textcolor(iFigFg); textbackground(iFigBg); cputs(sFigChar);
    if(flgMusic)
      for(i=1000; i>=500; i-=50)
	Sound(i, 0.3);
    Wait(50-(Timer()-t)*1000);
    t=Timer();
  }
  gotoxy(x, STAKAN_Y+STAKAN_HEIGHT); textcolor(7); textbackground(0); cputs("  ");
}

void ExHBuilder(void)
{ double t=Timer();
  int iXL=iFigX, iXR=iFigX;
  bool flgFirstIteration=true;
  bool f=false;
  textcolor(iFigFg); textbackground(iFigBg); gotoxy(wherex(), STAKAN_Y+iFigY);
  CellSet(iFigX, iFigY, 'ƒ');
  do
  { if(f)
    { if(iXL)
      { gotoxy(STAKAN_X+iXL*2, wherey()); putch(sFigChar[0]); putch('ƒ'); }
      if(iXR)
      { gotoxy(STAKAN_X+iXR*2, wherey()); putch('ƒ'); putch(sFigChar[1]); }
    }
    else
    { if(iXL)
      { gotoxy(STAKAN_X+iXL*2, wherey()); putch('ƒ');
	iXL--;
	if(iStakan[iXL+1][iFigY])
	  iXL=0;
	else
	{ CellSet(iXL, iFigY, 'ƒ');
	  gotoxy(STAKAN_X+iXL*2+1, wherey()); putch(sFigChar[0]);
	}
      }
      if(iXR)
      { gotoxy(STAKAN_X+iXR*2+1, wherey()); putch('ƒ');
	iXR++;
	if(iStakan[iXR+1][iFigY])
	 iXR=0;
	else
	{ CellSet(iXR, iFigY, 'ƒ');
	  gotoxy(STAKAN_X+iXR*2, wherey()); putch(sFigChar[1]);
	}
      }
    }
    f=!f;
    if(flgMusic && (!flgPlaying || flgFirstIteration)) Play("mbl64o5cdffo3");
    if(flgFirstIteration) flgFirstIteration=false;
    Wait(10-(Timer()-t)*1000);
    t=Timer();
  } while(iXL || iXR);
}

void ExPress(void)
{ double t=Timer();
  int iStartY=iFigY+1, x=STAKAN_X+iFigX*2, y, y2, i;
  for(y=iStartY; y<=STAKAN_HEIGHT; ++y)
    if(!iStakan[iFigX+1][y])
    { ScoreAdd(5);
      if(flgMusic)
	for(i=500; i<=1000; i+=50)
	  Sound(i, 0.3);
      for(y2=y; y2>=iStartY; --y2)
      { iStakanFg[iFigX-1][y2]=iStakanFg[iFigX-1][y2-1];
	iStakanBg[iFigX-1][y2]=iStakanBg[iFigX-1][y2-1];
	iStakan[iFigX+1][y2]=iStakan[iFigX+1][y2-1];
	gotoxy(x, STAKAN_Y+y2);
	if(y2==iStartY)
	{ textcolor(iFigFg); textbackground(iFigBg); cputs(sFigChar);
	  gotoxy(x, STAKAN_Y+y2-1);
	}
	textcolor(iStakanFg[iFigX-1][y2]); textbackground(iStakanBg[iFigX-1][y2]);
	putch(iStakan[iFigX+1][y2]); putch(iStakan[iFigX+1][y2]);
      }
      Wait(10-(Timer()-t)*1000);
      t=Timer();
      iStartY++;
    }
  gotoxy(x, STAKAN_Y+iStartY-1); textcolor(7); textbackground(0); cputs("  ");
}

void ExUnknown(void)
{ int iExN=Random(EX_MAX);
  iFigN=100+iExN; strcpy(sFigChar, sExChar[iExN]);
  iFigFg=iExFg[iExN]; iFigBg=iExBg[iExN];
  gotoxy(STAKAN_X+iFigX*2, STAKAN_Y+iFigY);
  textcolor(iFigFg); textbackground(iFigBg); cputs(sFigChar);
  if(flgMusic) Play("mbl64o1abcdaccfo3cddf");
  Wait(1000);
}

void ExVBuilder(void)
{ double t=Timer();
  int x=STAKAN_X+iFigX*2, y;
  for(y=iFigY+1; y<=STAKAN_HEIGHT; ++y)
    if(!iStakan[iFigX+1][y])
    { CellSet(iFigX, y, '≥');
      if(flgMusic && (!flgPlaying || y==iFigY+1)) Play("mbl64ffdc");
      ScoreAdd(5);
      gotoxy(x, STAKAN_Y+y); textcolor(iFigFg); textbackground(iFigBg); cputs("≥≥");
      Wait(10-(Timer()-t)*1000); t=Timer();
    }
    else if(iStakan[iFigX][y]=='ƒ')
    { CellSet(iFigX, y, '≈');
      gotoxy(x, STAKAN_Y+y); textcolor(iFigFg); textbackground(iFigBg); cputs("≈≈");
    }
  gotoxy(x, STAKAN_Y+iFigY); textcolor(7); textbackground(0); cputs("  ");
}

char *FieldInput(int x, int y, const char *sDefault, size_t Size)
{ char a, *b=malloc(Size+1);
  int i;
  if(!b)
  { fprintf(stderr, "Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  gotoxy(x, y); cputs(sDefault);
  gotoxy(x, y);
  SetCursorType(_SOLIDCURSOR);
#ifdef __MT__
  if(!flgKbdPoll)
  {
#endif
    KbdBufClean();
    a=KeyGet(true);
#ifdef __MT__
  }
  else
  { WaitThread(&KbdPollTID);
    a=cChar;
  }
#endif
  if(a=='\r')
  { SetCursorType(_NOCURSOR);
    return strcpy(b, sDefault);
  }
  b[0]='\0';
  for(i=0; i<Size; ++i)
    putch(' ');
  while(1)
  { switch(a)
    { case 27:
	gotoxy(x, y);
	for(i=0; i<strlen(b); i++)
	  putch(' ');
	if(flgMusic && !flgPlaying) printf("\a");
	b[0]='\0';
	SetCursorType(_NOCURSOR);
	return b;
      case '\r':
	SetCursorType(_NOCURSOR);
	return LTrim(RTrim(b));
      case '\b':
	if(b[0])
	{ b[strlen(b)-1]='\0'; x--; gotoxy(x, y); putch(' '); }
	break;
      default:
	if(a>=' ' && a<='~' && strlen(b)<Size)
	{ b[strlen(b)+1]='\0'; b[strlen(b)]=a; gotoxy(x, y); putch(a); ++x; }
	break;
    }
    gotoxy(x, y); a=KeyGet(true);
  }
  return LTrim(RTrim(b));
}

void FigCheck(int dx, int dy)
{ int n=0;
  int x=dx+iFigX, y=dy+iFigY;
  if(iFigN<100)
  { for(n=0; n<4; ++n)
      if(iStakan[x+iFigsX[iFigN][n]+1][y+iFigsY[iFigN][n]])
      { flgCollide=true;
	return;
      }
  }
  else if(iStakan[x+1][y]) flgCollide=true;
}

void FigDraw(void)
{ int y=STAKAN_Y+iFigY, x=STAKAN_X+iFigX*2;
  int n;
  if(iFigN>=0 && iFigN<=99)
  { textcolor(iFigFg); textbackground(iFigBg);
    for(n=0; n<4; ++n)
    { gotoxy(x+iFigsX[iFigN][n]*2, y+iFigsY[iFigN][n]); cputs(sFigChar); }
    return;
  }
  if(iFigN>=100 && iFigN<=104)
  { gotoxy(x, y); textcolor(iFigFg); textbackground(iFigBg);
    cputs(sFigChar);
    return;
  }
  if(iFigN==105)
  { gotoxy(x, y);
    textcolor(iFigFg+BLINK); textbackground(iFigBg); putch(sFigChar[0]);
    textcolor(iFigBg+BLINK); textbackground(iFigFg); putch(sFigChar[1]);
  }
}

void FigMove(int dx, int dy)
{ FigCheck(dx, dy);
  if(!flgCollide)
  { FigUnDraw(); iFigX=iFigX+dx; iFigY=iFigY+dy; FigDraw(); }
}

void FigNext(void)
{ char sFigChars[]=" ∞±≤€";
  int iExN;
  do
  { iFigN=iNxtN; iFigBg=iNxtBg; iFigFg=iNxtFg;
    strcpy(sFigChar, sNxtChar);
    iNxtN=0;
    if(iLevel==1)
      switch(Random(4))
      { case 0: iNxtFg=0; iNxtBg=7; break;
	case 1: iNxtFg=7; iNxtBg=0; break;
	case 2: iNxtFg=15; iNxtBg=7; break;
	case 3: iNxtFg=15; iNxtBg=0; break;
      }
    else
    { iNxtFg=Random(16); iNxtBg=Random(8);
      if(!(iNxtFg || iNxtBg)) iNxtFg=7;
      if(!flgClassic && iLevel>2 && Rnd()<0.2)
      { iExN=(int)(sqrt(Rnd())*(double)(iLevel-2))-1;
	if(iExN<0)
	  iNxtFg+=BLINK;
	else if(iExN<=EX_MAX)
	{ iNxtN=100+iExN; strcpy(sNxtChar, sExChar[iExN]);
	  iNxtFg=iExFg[iExN]; iNxtBg=iExBg[iExN];
	}
      }
    }
    if(!iNxtN)
    { iNxtN=Random(19);
      sNxtChar[0]=sNxtChar[1]=sFigChars[Random(5)];
      if(sNxtChar[0]==' ' && !iNxtBg) iNxtBg=7;
      if(sNxtChar[0]=='€' && !iNxtFg) iNxtFg=7;
    }
  } while(iFigN<0);
  if(flgNext) NxtDraw();
  iFigX=STAKAN_WIDTH/2; iFigY=0;
}

void FigRotate(void)
{ int n;
  if(iFigN>99) return;
  n=iFigN; FigUnDraw();
   iFigN=iFigsRot[iFigN]; FigCheck(0, 0);
  if(flgCollide)
    iFigN=n;
  FigDraw();
}

void FigSave(void)
{ int n;
  if(iFigN>=0 && iFigN<100)
  { for(n=0; n<4; ++n)
      CellSet(iFigX+iFigsX[iFigN][n], iFigY+iFigsY[iFigN][n], sFigChar[0]);
    return;
  }
  if(iFigN==105) ExUnknown();
  switch(iFigN)
  { case 100: ExBomb(); break;
    case 101: ExPress(); break;
    case 102: ExBurner(); break;
    case 103: ExHBuilder(); break;
    case 104: ExVBuilder(); break;
  }
}

void FigUnDraw(void)
{ int y=STAKAN_Y+iFigY, x=STAKAN_X+iFigX*2;
  int n;
  textcolor(7); textbackground(0);
  if(iFigN<100)
  { for(n=0; n<4; ++n)
    { gotoxy(x+iFigsX[iFigN][n]*2, y+iFigsY[iFigN][n]); cputs("  "); }
    return;
  }
  gotoxy(x, y); cputs("  ");
}

void GameOver(void)
{ static const char sGameOver[9][71]=
  { "011110001111110110000000110111111000111111101100110111111011111000111",
    "1122110011221121111000111121122222001122211211201121122222112221101112",
    "1120022011201121121101121121111110001120011211201121111110112001121112",
    "1120000011111121120111221121122222001120011211201121122222111110220222",
    "1120111011221121120022201121120000001120011211201121120000112211",
    "011112121120112112000000112111111000111111120111122111111011200110111",
    "0022220202200220220000000220222222000222222200222200222222022000220222",
    "111111111111111111111111111111111111111111111111111111111111111111111",
    "0222222222222222222222222222222222222222222222222222222222222222222222"
  };
  int i, j;
  if(flgMusic) Play("mbl7cp15cp25l6ccp15l7ep15ddp15ccp15bp25c");
  for(i=12; i<=20; ++i)
    for(j=0; j<strlen(sGameOver[i-12]); ++j)
    { gotoxy(5+j, i);
      switch(sGameOver[i-12][j])
      { case '1': textcolor(12); textbackground(12); putch('€'); break;
	case '2': textcolor(0);  textbackground(4);  putch('≤'); break;
      }
    }
#ifdef __MT__
  if(!flgKbdPoll)
#endif
    KeyGet(true);
#ifdef __MT__
  else
    WaitThread(&KbdPollTID);
#endif
  TableWrite();
}

void GamePause(void)
{ int x=STAKAN_X+STAKAN_WIDTH-2, y=STAKAN_HEIGHT/2-1, i;
  void *pBkgr=malloc(3*9*2);
  struct text_info oldtextinfo;
  gettextinfo(&oldtextinfo);
  if(!pBkgr)
  { fprintf(stderr, "Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  gettext(x, STAKAN_Y+y, x+8, STAKAN_Y+y+2, pBkgr);
  gotoxy(x, STAKAN_Y+y); textcolor(7); textbackground(0);
  cputs("˙˙˙˙˙˙˙˙˙");
  gotoxy(x, STAKAN_Y+y+1); putch('˙'); textcolor(BLINK+14); cputs(" P");
  textcolor(BLINK+15); cputs("AUSE "); textcolor(7); putch('˙');
  gotoxy(x, STAKAN_Y+y+2); cputs("˙˙˙˙˙˙˙˙˙");
#ifdef __MT__
  if(!flgKbdPoll)
  {
#endif
    KbdBufClean();
    KeyGet(true);
#ifdef __MT__
  }
  else
    WaitThread(&KbdPollTID);
#endif
  puttext(x, STAKAN_Y+y, x+8, STAKAN_Y+y+2, pBkgr);
  free(pBkgr);
  textattr(oldtextinfo.attribute);
}

void GameStart(void)
{ const char b[]="Congratulations!!!  You have the best Tetris in the world.";
  const lxmax=58;
  int iColor, lx, i, n;
  bool flgExitWhile;
  char dl, *s, a[59];
  double t=Timer();
  while(1)
  { textcolor(7); textbackground(0);
    clrscr();
    Sound(100, 0);
    if(flgMusic) Play("mbl8cc#dd#dc#ccc#dd#dc#ccc#dd#dc#cp4cp4c");
    textcolor(4);
    gotoxy(6, 1);    cputs("‹€€€€€€€€‹         ‹€€€€€€€€‹         ‹€€€€€€€€‹         ‹€€€€€€€€‹");
    gotoxy(4, 2);  cputs("‹€€ﬂ      ﬂ€€‹     ‹€€ﬂ      ﬂ€€‹     ‹€€ﬂ      ﬂ€€‹     ‹€€ﬂ      ﬂ€€‹");
    textcolor(2);
    gotoxy(3, 3); cputs("€€›          ﬁ€€   €€›          ﬁ€€   €€›          ﬁ€€   €€›          ﬁ€€");
    gotoxy(3, 4); cputs("€€€€€€€€€€€€€€€€   €€€€€€€€€€€€€€€€   €€€€€€€€€€€€€€€€   €€€€€€€€€€€€€€€€");
    textcolor(1);
    gotoxy(3, 5); cputs("€€›    ‹€€ﬂ  ﬁ€€   €€›    ‹€€ﬂ  ﬁ€€   €€›    ‹€€ﬂ  ﬁ€€   €€›    ‹€€ﬂ  ﬁ€€");
    gotoxy(3, 6); cputs("€€›  ‹€€ﬂ    ﬁ€€   €€›  ‹€€ﬂ    ﬁ€€   €€›  ‹€€ﬂ    ﬁ€€   €€›  ‹€€ﬂ    ﬁ€€");
    textcolor(13);
    gotoxy(18, 18); cputs("Copyright (C) 29-May-99 by SUE, hackers group.");
    gotoxy(37, 19); cputs("to see top scores.");
    gotoxy(37, 20); cputs("to start htetris.");
    gotoxy(24, 21); cputs("PRESS        to start classic tetris.\r\n");
    if(iMyLevelMax>1)
    { gotoxy(37, 22); cputs("to start this level\r\n"); }
    gotoxy(37, wherey()); cputs("to end game.");
    textcolor(14);
    gotoxy(32, 19); cputs("F3");
    gotoxy(31, 20); cputs("Space");
    gotoxy(32, 21); cputs("F10\r\n");
    if(iMyLevelMax>9) n=55; else n=48;
    if(iMyLevelMax>1) { gotoxy(31, 22); cputs("1..."); putch(iMyLevelMax+n); cputs("\r\n"); }
    gotoxy(32, wherey()); cputs("Esc");
    KbdBufClean();
    while(1)
    { flgExitWhile=false;
      iColor=1+Random(14);
      if(iColor==7) iColor=2;
      textcolor(iColor);
      gotoxy(15, 10); cputs("€€      €€€€€€€€ €€€€€€ €€€€€€€€ €€€€€€  €€ €€€€€€");
      gotoxy(15, 11); cputs("€€         €€    €€        €€    €€   €€ €€ €€");
      gotoxy(15, 12); cputs("€€€€€€€    €€    €€€€€€    €€    €€€€€€  €€ €€€€€€");
      gotoxy(15, 13); cputs("€€   €€    €€    €€        €€    €€€€    €€     €€");
      gotoxy(15, 14); cputs("€€   €€    €€    €€€€€€    €€    €€  €€  €€ €€€€€€");
      textcolor(12);
      for(lx=1; lx<=lxmax; ++lx)
      { strncpy(a, b, lx);
	a[lx]='\0';
	gotoxy(11, 9); cputs(a);
	gotoxy(11, 15); cputs(a);
	Wait(9-(Timer()-t)*1000);
	t=Timer();
	switch(dl=toupper(KeyGet(false)))
	{ case 27:
	    textcolor(7); textbackground(0);
	    gotoxy(57, 12); cputs("∞∞");
	    gotoxy(57, 13); cputs("∞∞");
	    gotoxy(24, 14); cputs("∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞");
	    textcolor(0); textbackground(7);
	    gotoxy(22, 11); cputs(" …ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª ");
	    gotoxy(22, 12); cputs(" ∫ Do you want to exit (Y/N) ?   ∫ ");
	    gotoxy(22, 13); cputs(" »ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº ");
	    textcolor(15); textbackground(0);
	    if(!strcmp(UpperStr(s=FieldInput(53, 12, "n", 1)), "Y"))
	    { free(s);
	      CursorRestore();
#ifdef __MT__
	      CloseEventSem(hKeyPressSem);
#endif
	      textmode(oldtextinfo.currmode);
	      normvideo();
	      clrscr();
	      exit(EXIT_SUCCESS);
	    }
	    free(s);
	    flgExitWhile=true;
	    break;
	  case ' ':
	    flgClassic=false; iLevel=1; Sound(100, 0); return;
	  case KG_KEYF10:
	    flgClassic=true; iLevel=1; Sound(100, 0); return;
	  case KG_KEYF3:
	    TableRead(true); flgExitWhile=true; break;
	  default:
	    if((dl>='1' && dl<='9') || (dl>='A' && dl<='Z'))
	    { if(dl<'A') n=dl-'0'; else n=dl-55;
	      if(iMyLevelMax>1 && n<=iMyLevelMax)
	      { iLevel=n;
		Sound(100, 0);
		return;
	      }
	    }
	    break;
	}
	if(flgExitWhile) break;
      }
      if(flgExitWhile) break;
      Wait(89-(Timer()-t)*1000);
      t=Timer();
      gotoxy(11, 9); 
      cputs("                                                          ");
      gotoxy(11, 15);
      cputs("                                                          ");
    }
  }
}

void InitMusic(void)
{ if(_argc>1 && !(strcmp(_argv[1], "/m") && strcmp(_argv[1], "/M") &&
                  strcmp(_argv[1], "-m") && strcmp(_argv[1], "-M")))
    flgMusic=false;
}

void iSwap(int *i1, int *i2)
{ int t=*i2;
  *i2=*i1;
  *i1=t;
}

#ifdef __MT__
void KbdPollThread(void *args)
{ flgKbdPoll=true;
  cChar=KeyGet(true);
  PostEventSem(hKeyPressSem);
  flgKbdPoll=false;
}
#endif

void LevelInc(void)
{ iLevel+=1;
  textcolor(11); textbackground(12);
  gotoxy(STAKAN_X+2, STAKAN_HEIGHT/2+STAKAN_Y-1); cputs("…ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª");
  gotoxy(STAKAN_X+2, STAKAN_HEIGHT/2+STAKAN_Y);   cputs("∫                  ∫");
  gotoxy(STAKAN_X+2, STAKAN_HEIGHT/2+STAKAN_Y+1); cputs("∫                  ∫");
  gotoxy(STAKAN_X+2, STAKAN_HEIGHT/2+STAKAN_Y+2); cputs("∫                  ∫");
  gotoxy(STAKAN_X+2, STAKAN_HEIGHT/2+STAKAN_Y+3); cputs("»ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº");
  textcolor(BLINK+14);
  gotoxy(STAKAN_X+4, STAKAN_HEIGHT/2+STAKAN_Y); cputs("Congratulations!");
  textcolor(1);
  gotoxy(STAKAN_X+5, STAKAN_HEIGHT/2+STAKAN_Y+1); cputs("Entering");
  gotoxy(STAKAN_X+10, STAKAN_HEIGHT/2+STAKAN_Y+2); cputs("Level ");
  textcolor(14); cprintf("%d", iLevel);
  if(flgMusic) Play("mbl10cdegp15eg");
  textcolor(12); textbackground(1);
  gotoxy(LEVEL_X, LEVEL_Y); cprintf("%6d", iLevel);
#ifdef __MT__
  if(!flgKbdPoll)
  {
#endif
    KbdBufClean();
    KeyGet(true);
#ifdef __MT__
  }
  else
    WaitThread(&KbdPollTID);
#endif
  textcolor(14); textbackground(1);
  gotoxy(LEVEL_X, LEVEL_Y); cprintf("%6d", iLevel);
}

void LevelPrint(void)
{ int i;
  if(flgClassic) return;
  gotoxy(LVLTXT_X, LVLTXT_Y); textcolor(14); textbackground(0);
  for(i=0; i<=LEVEL_NAME_LEN_MAX; ++i)
    putch(' ');
  gotoxy(LVLTXT_X, LVLTXT_Y);
  if(iLevel>LEVEL_NAME_MAX)
    cprintf("%s %d", sLevelNames[LEVEL_NAME_MAX-1], iLevel-LEVEL_NAME_MAX);
  else
    cputs(sLevelNames[iLevel-1]);
}

void LinesCheck(void)
{ int iDeltaScores=10, x, y, yup;
  bool f=false;
  double t=Timer();
  for(yup=1; yup<=STAKAN_HEIGHT; ++yup)
  { for(x=1; x<=STAKAN_WIDTH; ++x)
      if(iStakan[x+1][yup])
      { f=true; break; }
    if(f) break;
  }
  for(y=yup; y<=STAKAN_HEIGHT; ++y)
  { f=true;
    for(x=1; x<=STAKAN_WIDTH; ++x)
      if(!iStakan[x+1][y])
      { f=false; break; }
    if(f)
    { textcolor(BLINK+14); textbackground(1);
      for(x=1; x<=STAKAN_WIDTH; ++x)
      { gotoxy(STAKAN_X+2*x, STAKAN_Y+y); cputs("**");
	Wait(10-(Timer()-t)*1000);
	t=Timer();
      }
      if(flgMusic) Play("mbl64gefbdca");
      iLines++;
      textcolor(14); textbackground(1);
      gotoxy(LINES_X, LINES_Y); cprintf("%6d", iLines);
      if(iLines-iOldLines>20) flgLevel=true;
      ScoreAdd(iDeltaScores); iDeltaScores+=10;
      LinesDraw(yup-1, y, true);
    }
  }
}

void LinesDraw(int ys, int ye, bool flgDelete)
{ int iX, iY;
  for(iY=ye; iY>=ys; --iY)
    for(iX=1; iX<=STAKAN_WIDTH; ++iX)
      if(flgDelete)
      { iStakan[iX+1][iY]=iStakan[iX+1][iY-1];
	iStakanFg[iX-1][iY]=iStakanFg[iX-1][iY-1];
	iStakanBg[iX-1][iY]=iStakanBg[iX-1][iY-1];
      }
      else
      { gotoxy(STAKAN_X+2*iX, STAKAN_Y+iY);
	textcolor(iStakanFg[iX-1][iY]); textbackground(iStakanBg[iX-1][iY]);
	putch(iStakan[iX+1][iY]); putch(iStakan[iX+1][iY]);
      }
  if(!flgDelete) return;
  movetext(STAKAN_X+2, STAKAN_Y+ys-1, STAKAN_X+2*STAKAN_WIDTH+1, STAKAN_Y+ye-1,
	   STAKAN_X+2, STAKAN_Y+ys);
}

void LinesInit(void)
{ int x, y;
  iFigN=iNxtN=-1;
  for(x=1; x<=STAKAN_WIDTH; ++x)
  { iStakan[x+1][0]=iStakanFg[x-1][0]=iStakanBg[x-1][0]=0;
    iStakan[x+1][STAKAN_HEIGHT+1]=2;
  }
  for(y=0; y<=STAKAN_HEIGHT; ++y)
  { iStakan[1][y]=iStakan[STAKAN_WIDTH+2][y]=2;
    for(x=1; x<=STAKAN_WIDTH; ++x)
      iStakan[x+1][y]=iStakanFg[x-1][y]=iStakanBg[x-1][y]=0;
  }
}

char *LTrim(char *s)
{ int i, len=strlen(s);
  for(i=0; i<len && s[i]==' '; ++i);
  if(!i) return s;
  if(i==len) { s[0]='\0'; return s; }
  memmove(s, s+i, len-i+1);
  return s;
}

void NxtDraw(void)
{ int n;
  textcolor(7); textbackground(0);
  for(n=0; n<4; ++n)
  { gotoxy(NXT_X, NXT_Y+n); cputs("        "); }
  textcolor(iNxtFg); textbackground(iNxtBg);
  if(iNxtN>=0 && iNxtN<=99)
  { for(n=0; n<4; ++n)
    { gotoxy(NXT_X+iFigsX[iNxtN][n]*2, NXT_Y+iFigsY[iNxtN][n]);
      cputs(sNxtChar);
    }
    return;
  }
  if(iNxtN>=100 && iNxtN<=104)
  { gotoxy(NXT_X+3, NXT_Y+2); cputs(sNxtChar); return; }
  gotoxy(NXT_X+3, NXT_Y+2);
  textcolor(iNxtFg+BLINK); textbackground(iNxtBg); putch(sNxtChar[0]);
  textcolor(iNxtBg+BLINK); textbackground(iNxtFg); putch(sNxtChar[1]);
}

void NxtFrame(bool flgParam)
{ int i;
  textbackground(0);
  if(flgParam) textcolor(1); else textcolor(0);
  gotoxy(NXT_X-2, NXT_Y-2); cputs("…ÕÕÕÕÕÕÕÕÕÕª");
  for(i=-1; i<4; ++i)
  { gotoxy(NXT_X-2, NXT_Y+i); cputs("∫          ∫"); }
  gotoxy(NXT_X-2, NXT_Y+i); cputs("»ÕÕÕÕÕÕÕÕÕÕº");
}

void NxtSwap(void)
{ FigUnDraw();
  iSwap(&iFigN, &iNxtN); FigCheck(0, 0);
  if(flgCollide)
    iSwap(&iFigN, &iNxtN);
  else
  { iSwap(&iFigBg, &iNxtBg); iSwap(&iFigFg, &iNxtFg); sSwap(sFigChar, sNxtChar);
    NxtDraw();
  }
  FigDraw();
}

void Play(const char *PlayString)
{
#ifdef __MT__
  char *tmp;
  if(flgPlaying)
  { KillThread(PlayTID);
    flgPlaying=false;
    if(sBuffer) { free(sBuffer); sBuffer=NULL; }
    if(sTemp)   { free(sTemp);   sTemp=NULL;   }
  }
#endif
  if(PlayString[0]=='m' && PlayString[1]=='b')
  {
#ifdef __MT__
    flgPlaying=true;
    tmp=malloc(strlen(PlayString)+1);
    strcpy(tmp, PlayString);
    PlayTID=BeginThread(PlayThread, 8192, tmp);
    while(!sBuffer);
    free(tmp);
#endif
  }
  else
    PlayThread(PlayString);
}

void PlayThread(void *PlayString)
{ static const double k=1.059463094359;
  int oct=3;
  int note, n=1, tempo=128, p;
  int msec=1000*(240.0/128.0);
  int i, j, iBufLen;
  double t=Timer();
#ifdef __MT__
  CritSecEnter();
#endif
  iBufLen=strlen((char *)PlayString);
  sBuffer=malloc(iBufLen+1);
  sTemp=malloc(iBufLen+1);
  strcpy(sBuffer, (char *)PlayString);
  for(i=0; i<iBufLen; ++i)
    switch(sBuffer[i])
    { case 'm':
	if(i+1<iBufLen)
	  if(sBuffer[i+1]=='f' || sBuffer[i+1]=='b')
	    ++i;
	break;
      case 'o':
	if(i+1<iBufLen)
	{ i++;
	  if(isdigit(sBuffer[i]))
	    oct=sBuffer[i]-'0';
	}
	break;
      case 'l':
	if(i+1<iBufLen)
	{ for(j=0, i++; isdigit(sBuffer[i]); ++i, ++j)
	    sTemp[j]=sBuffer[i];
	  sTemp[j]='\0';
	  --i;
	  sscanf(sTemp, "%d", &n);
	  msec=1000*(240.0/((double)tempo*(double)n));
	}
	break;
      case 't':
	for(j=0, i++; isdigit(sBuffer[i]); ++i, ++j)
	  sTemp[j]=sBuffer[i];
	sTemp[j]='\0';
	--i;
	sscanf(sTemp, "%d", &tempo);
	msec=1000*(240.0/((double)tempo*(double)n));
	break;
      case '>':
	if(oct<6)
	  ++oct;
	break;
      case '<':
	if(oct>0)
	  --oct;
	break;
      case 'p':
	if(i+1<iBufLen)
	  for(j=0, i++; isdigit(sBuffer[i]); ++i, ++j)
	    sTemp[j]=sBuffer[i];
	  sTemp[j]='\0';
	  --i;
	  sscanf(sTemp, "%d", &p);
#ifdef __MT__
	  CritSecExit();
#endif
	  Wait(1000*(240.0/((double)tempo*(double)p)-(Timer()-t)));
#ifdef __MT__
	  CritSecEnter();
#endif
	  t=Timer();
	break;
      default:
	if('a'<=sBuffer[i] && 'g'>=sBuffer[i])
	{ note=sBuffer[i]-'a';
	  switch(note)
	  { case 1:
	    case 2:
	      ++note; break;
	    case 3:
	      note+=2; break;
	    case 4:
	    case 5:
	      note+=3; break;
	    case 6:
	      note+=4; break;
	  }
	  if(sBuffer[i+1]=='#') ++note;
#ifdef __MT__
	  CritSecExit();
#endif
	  Beep(pow(k, (double)(12*oct+note))*110.0, msec);
#ifdef __MT__
	  CritSecEnter();
#endif
	  t=Timer();
	}
	break;
    }
  free(sBuffer);
  free(sTemp);
  sBuffer=sTemp=NULL;
#ifdef __MT__
  flgPlaying=false;
  CritSecExit();
#endif
}

char *RTrim(char *s)
{ int i, l=strlen(s);
  for(i=l; l>0 && s[i-1]==' '; --i);
  if(i==l) return s;
  if(!i) { s[0]='\0'; return s; }
  s[i]='\0';
  return s;
}

void ScoreAdd(int i)
{ lScore+=i;
  textcolor(BLINK+14); textbackground(1);
  gotoxy(SCORE_X, SCORE_Y); cprintf("%6d", lScore);
  if(lScore-lOldScore>1000) flgLevel=true;
}

void sSwap(char *s1, char *s2)
{ char *t;
  int ml;
  ml=strlen(s1)+1;
  if(strlen(s2)+1>ml) ml=strlen(s2)+1;
  t=malloc(ml);
  if(!t)
  { fprintf(stderr, "Not enough memory\n");
    exit(EXIT_FAILURE);
  }
  strcpy(t, s2);
  strcpy(s2, s1);
  strcpy(s1, t);
  free(t);
}

void TableRead(bool flgKeyWait)
{ int i;
  FILE *fp=fopen("htetris.scr", "r");
  iPlayersNum=-1;
  for(i=1; i<=8; ++i)
  { fgets(sNames[i], 99, fp);
    *strchr(sNames[i], '\n')='\0';
    if(!sNames[i][0] && iPlayersNum==-1)
      iPlayersNum=i-1;
  }
  if(iPlayersNum==-1) iPlayersNum=8;
  for(i=1; i<=8; ++i)
    fscanf(fp, "%ld", &lScores[i]);
  for(i=1; i<=8; ++i)
    fscanf(fp, "%d", &iLevels[i]);
  fclose(fp);
  TableShow(flgKeyWait);
}

void TableRemove(int iStart, int iNumber, int iDirection)
{ int t;
  for(t=iStart; iDirection>0 ? t<=iNumber : t>=iNumber; t+=iDirection)
  { lScores[t]=lScores[t+iDirection];
    strcpy(sNames[t], sNames[t+iDirection]);
    iLevels[t]=iLevels[t+iDirection];
  }
}

void TableShow(bool flgKeyWait)
{ int i;
  textcolor(7); textbackground(0);
  for(i=0; i<=iPlayersNum+1; ++i)
  { gotoxy(58, 13+i); cputs("∞∞"); }
  if(!iPlayersNum)
  { gotoxy(23, 15); cputs("∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞"); }
  else
  { gotoxy(23, 14+iPlayersNum); cputs("∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞"); }
  textbackground(1);
  gotoxy(21, 12); cputs(" …ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª ");
  for(i=1; i<=iPlayersNum; ++i)
  { gotoxy(21, 12+i); cputs(" ∫                                 ∫ "); }
  if(!iPlayersNum)
  { gotoxy(21, 13); cputs(" ∫       The table is empty.       ∫ ");
    gotoxy(21, 14); cputs(" »ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº ");
  }
  else
  { gotoxy(21, 13+iPlayersNum); cputs(" »ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº "); }
  for(i=1; i<=iPlayersNum; ++i)
  { switch(i)
    { case 1: textcolor(14); break;
      case 2: textcolor(9); break;
      case 3: textcolor(10); break;
      case 4: textcolor(12); break;
      default: textcolor(7);
    }
    gotoxy(23, i+12); cprintf("%2d", i);
    gotoxy(26, i+12); cprintf(" %.17s", sNames[i]);
    gotoxy(43, i+12); cprintf("%8ld", lScores[i]);
    gotoxy(51, i+12); cprintf("%4d", iLevels[i]);
  }
  if(flgKeyWait)
  { gotoxy(30, 12); textcolor(7); textbackground(1); cputs(" Press any key. "); KeyGet(true); }
}

void TableWrite(void)
{ const int x1=16, y1=2;
  int i, j, t;
  char *u1, *u2, *sName, sOldPlayerName[18];
  FILE *fp;
  TableRead(false);
  for(i=1; i<=8; ++i)
    if(lScore>lScores[i])
    { textcolor(7); textbackground(0);
      for(j=1; j<=4; ++j)
      { gotoxy(x1+46, y1+j); cputs("∞∞"); }
      gotoxy(x1, y1+5); cputs("∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞");
      textbackground(1);
      gotoxy(x1-2, y1);   cputs(" …ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª ");
      gotoxy(x1-2, y1+1); cputs(" ∫                                            ∫ ");
      gotoxy(x1-2, y1+2); cputs(" ∫                                            ∫ ");
      gotoxy(x1-2, y1+3); cputs(" ∫                                            ∫ ");
      gotoxy(x1-2, y1+4); cputs(" »ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº ");
      textcolor(14);
      gotoxy(x1, y1+1); cputs("CONGRATULATIONS! YOU GOT A           SCORES.");
      textcolor(BLINK+14);
      gotoxy(x1+27, y1+1); cputs("TOP EIGHT");
      textcolor(14);
      gotoxy(x1+11, y1+2); cputs("ENTER YOUR NAME.");
      sName=RTrim(LTrim(FieldInput(x1+14, y1+3, sPlayerName, 16)));
      if(!sName[0]) return;
      strcpy(sOldPlayerName, sPlayerName);
      strcpy(sPlayerName, sName);
      free(sName);
      if(strcmp(u1=UpperStrN(sOldPlayerName), u2=UpperStrN(sPlayerName)))
	iMyLevelMax=iLevel;
      else if(iMyLevelMax<iLevel) iMyLevelMax=iLevel;
      free(u1);
      for(t=1; t<=8; t++)
      {	if(!strcmp(u1=UpperStrN(sNames[t]), u2))
	{ if(iMyLevelMax<iLevels[t]) iMyLevelMax=iLevels[t];
	  if(lScores[t]>=lScore) return;
	  if(t<=7) TableRemove(t, 7, 1);
	  if(iPlayersNum<=7) iPlayersNum--;
	}
	free(u1);
      }
      free(u2);
      textcolor(1); textbackground(0);
      TableRemove(8, i, -1);
      lScores[i]=lScore;
      strcpy(sNames[i], sPlayerName);
      iLevels[i]=iLevel;
      if(iPlayersNum<8) iPlayersNum++;
      break;
    }
  fp=fopen("htetris.scr", "w");
  for(i=1; i<=8; ++i)
    fprintf(fp, "%s\n", sNames[i]);
  for(i=1; i<=8; ++i)
    fprintf(fp, "%ld\n", lScores[i]);
  for(i=1; i<=8; ++i)
    fprintf(fp, "%d\n", iLevels[i]);
  fclose(fp);
  TableShow(true);
}

char *UpperStr(char *s)
{ int i;
  for(i=0; s[i]; ++i)
    s[i]=toupper(s[i]);
  return s;
}

char *UpperStrN(const char *s)
{ char *r=malloc(strlen(s)+1);
  if(!r)
  { fprintf(stderr, "Not enough memory\n"); exit(EXIT_FAILURE); }
  strcpy(r, s);
  return UpperStr(r);
}
