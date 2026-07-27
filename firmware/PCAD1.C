#include <graph.h>
#include <io.h>
#include <dos.h>
#include <math.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <menu.h>
#include "tb.h"
#define PI 3.14159

ITEM mnuMain[] =
{                    /* Highlight Char  Pos */
    { 0, "Quit"            },   /* Q     0  */
    { 0, "Init PC AD/DA"   },   /* I     0  */
    { 0, "PC DFT"          },   /* D     0  */
    { 0, "Datum "          },   /* D     0  */
    { 0, "Time"            },   /* T     0  */
    { 0, "PC Speech DVM"   },   /* S     0  */
    { 0, "PC Scope"        },   /* N     0  */
    { 0, "Tests"           },   /* T     0  */
    { 0, ""                }
};

ITEM mnuInit[] =
{                    /* Highlight Char  Pos */
    { 0, "Hauptmenu"       },   /* Q     0  */
    { 0, "Eingang"         },   /* E     0  */
    { 0, "PolaritÑt"       },   /* P     0  */
    { 0, "Ausgang"         },   /* A     0  */
    { 0, "Filter"          },   /* F     0  */
    { 0, "Gain"            },   /* G     0  */
    { 0, "Taktquelle"      },   /* T     0  */
    { 0, "Frequenz"        },   /* F     0  */
    { 0, "Display all"     },   /* D     0  */
    { 0, ""                }
};

ITEM mnuFilter[] =
{                    /* Highlight Char  Pos */
    { 7, "Filter EIN"      },   /* E     7  */
    { 7, "Filter AUS"      },   /* A     7  */
    { 7, ""                }
};

ITEM mnuFrequenz[] =
{                    /* Highlight Char  Pos   */
    { 18, "Filter =  1 kHz"},/* 1 18  */
    { 18, "Filter =  2 kHz"},/* 2 18  */
    { 18, "Filter =  3 kHz"},/* 3 18  */
    { 18, "Filter =  4 kHz"},/* 4 18  */
    { 18, "Filter =  5 kHz"},/* 5 18  */
    { 18, "Filter =  6 kHz"},/* 6 18  */
    { 18, "Filter =  7 kHz"},/* 7 18  */
    { 18, "Filter =  8 kHz"},/* 8 18  */
    { 18, "Filter =  9 kHz"},/* 9 18  */
    { 17, "Filter = 10 kHz"},/* 0 17  */
    { 17, "Filter = 11 kHz"},/* 1 17  */
    { 17, "Filter = 12 kHz"},/* 2 17  */
    { 17, "Filter = 13 kHz"},/* 3 17  */
    { 17, "Filter = 14 kHz"},/* 4 17  */
    { 17, "Filter = 15 kHz"},/* 5 17  */
    { 17, "Filter = 16 kHz"},/* 6 17  */
    { 17, "Filter = 17 kHz"},/* 7 17  */
    { 17, "Filter = 18 kHz"},/* 8 17  */
    { 17, "Filter = 19 kHz"},/* 9 17  */
    { 17, "Filter = 20 kHz"},/* 2 17  */
    { 17, ""                }
};

ITEM mnuGain[] =
{                    /* Highlight Char  Pos */
    { 10, "Gain =   1"    },     /* 1     9  */
    { 10, "Gain =   2"    },     /* 2     9  */
    { 10, "Gain =   4"    },     /* 4     9  */
    { 10, "Gain =   8"    },     /* 8     9  */
    { 10, "Gain =  16"    },     /* 1     8  */
    { 10, "Gain =  32"    },     /* 3     8  */
    { 10, "Gain =  64"    },     /* 6     8  */
    { 10, "Gain = 128"    },     /* 1     7  */
    { 10, ""              }
};

ITEM mnuTaktquelle[] =
{                    /* Highlight Char  Pos */
    { 1, " 1,44 MHZ "        },   /* 1     0  */
    { 1, " 8254     "        },   /* 8     0  */
    { 1, ""                  }
};

ITEM mnuEingang[] =
{                    /* Highlight Char  Pos */
    { 9, "Eingang  A"       },   /* A     8  */
    { 9, "Eingang  B"       },   /* B     8  */
    { 9, "Eingang  C"       },   /* C     8  */
    { 9, "Eingang  D"       },   /* D     8  */
    { 9, "Eingang  E"       },   /* E     8  */
    { 9, "Eingang  F"       },   /* F     8  */
    { 9, ""                 }
};  

ITEM mnuAusgang[]=
{
    { 9, "Ausgang  A"       },   /* A     9  */
    { 9, "Ausgang  B"       },   /* B     9  */
    { 4, "   A+B    "       },   /* +     4  */
    { 9, ""                 }
};

ITEM mnuScope[] =
{
    { 0, "Quit"            },   /* Q     0  */
    { 0, "0,1 V/cm"        },   /* Q     0  */
    { 0, "0,2 V/cm"        },   /* 0     0  */
    { 0, "0,5 V/cm"        },   /* 0     0  */
    { 0, "1 V/cm"          },   /* I     0  */
    { 0, "2 V/cm"          },   /* D     0  */
    { 0, ""                }
};

ITEM mnuTimebase[] =
{
    { 2, "  1 ms  "          },   /* 1     0  */
    { 2, "  2 ms  "          },   /* 2     0  */
    { 2, "  3 ms  "          },   /* 3     0  */
    { 2, "  4 ms  "          },   /* 4     0  */
    { 2, "  5 ms  "          },   /* 5     0  */
    { 2, "  6 ms  "          },   /* 6     0  */
    { 2, ""                }
};

ITEM mnuPolaritaet[] =
{                    /* Highlight Char  Pos */
    { 0, "Unipolar  "        },   /* U     0  */
    { 0, "Bipolar   "        },   /* B     0  */
    { 0, ""                  }
};

enum CHOICES
{
    QUIT, INIT, DFT, DATUM, TIME, SPEECHDVM, SCOPE, TESTS,
    HAUPTMENU, EINGANG, POLARITAET, AUSGANG, FILTER, GAIN, 
    TAKTQUELLE, FREQUENZ, DISALL
};

unsigned char *face[5]=
{
    "t'courier'","t'helv'","t'tms rmn'","t'modern'","t'roman'"
};

extern void dft();
extern void showdate();
extern void showtime();
extern int speech();
extern void scope();
void init();
void filter();
void frequenz();
void gain();
void taktquelle();
void eingang();
void polaritaet();
void ausgang();
extern void videoconfig();
void disall();

struct videoconfig vc;
int iMainCur=1,iInitCur=1,iFilterCur=0,iFrequenzCur=19,iGainCur=0,
    iTaktquelleCur=1,iEingangCur=0,iPolaritaetCur=1;iAusgangCur=2;
int kanal=0,kanaladr=0x280,REGISTER=0xd0,OUT=0x300,FILTERSELECT=0x20,
    FWERT=0x7f,INGAIN=0x00,TAKT=0x05;

#include "d:\c600\andi2\pcad2.c"

main()
{
/*******************************************************************/
/* Voreinstellung der Parameter fÅr die A/D-D/A Karte:             */
/* INGAIN = 0x00 d.h. gain ist gleich 1 (0x00-0x07 fÅr 1-128)      */
/* A/D EINGANG MODE1 =0 d.h single mode (0-1 fÅr single-differenz) */
/* A/D EINGANG MODE2 =1 d.h bipolar mode (0-1 fÅr bi-unipolar)     */
/* FILTER MODE =1 d.h filter ein (0-1 fÅr aus-ein)                 */
/* FWERT =0x7f d.h. Filterfreq =20KHz                              */
/* WANDLERTAKT =1 d.h vom 8254 (0-1 fÅr fest1,44MHz-variabel 8254) */
/* OUT =0x300 d.h beide outputs aktiv (0x100 fÅr out1-0x200 fÅr    */
/* out2-0x300 fÅr beide outputs                                    */
/* TAKT =0x06 d.h takt=8MHz/0x06=1,33MHz Wandlertakt               */
/*******************************************************************/

outpw(0x294,REGISTER+OUT);		/* Filter A */
outpw(0x292,FWERT);			/* Frequenzwert */
outpw(0x294,REGISTER+OUT+FILTERSELECT);	/* Filter B */
outpw(0x292,FWERT);			/* Frequenzwert */
outpw(0x294,REGISTER+OUT+INGAIN);	/* Takt timer0, Filter on, Bipolar */
outp(0x28f,0x3e);			/* Timer 0 initialisieren */
outp(0x28c,TAKT);			/* Takt fÅr A/D (1,6MHz) (05-0d) */
outp(0x28c,0x00);			/* CLK high Byte */
outp(0x28f,0x7e);			/* Timer 1 initialisieren */
outp(0x28d,0x7e);			/* 70h fÅr 14us */
outp(0x28d,0x00);			/* High Byte */
outp(0x29c,0x00);			/* NORMAL MODE */

_getvideoconfig(&vc);
if(!_setvideomode(_DEFAULTMODE))exit(1);
if(_registerfonts("*.FON")<0)
  {
   printf("Fehler, keine Fonts vorhanden !");
   exit(0);
  }
if(tb_init("v15 ff dn") eq RC_FAIL)
  {
   printf("Kann Soundblaster nicht initialisieren !");
   getch();
   exit(0);
  }

for(;;)
  {
   _setbkcolor((long)_TBLUE);
   _clearscreen(_GCLEARSCREEN);
   mnuAtrib.fgBorder=5;
   iMainCur=Menu(10,38,mnuMain,iMainCur);
   switch(iMainCur)
         {
          case QUIT:
            tb_string("ende",1);
            _setvideomode(_DEFAULTMODE);
            _unregisterfonts();
            tb_terminate();
            exit(0);
            break;
          case INIT:
            tb_string("initialisieren",1);
            _clearscreen(_GCLEARSCREEN);
            init();
            break;
          case DFT:
            _displaycursor(_GCURSOROFF);
            _clearscreen(_GCLEARSCREEN);
            dft();
            break;
          case DATUM:
            tb_string("datum",1);
            _displaycursor(_GCURSOROFF);
            showdate();
            break;
          case TIME:
            tb_string("uhrzeit",1);
            _displaycursor(_GCURSOROFF);
            showtime();
            break;
          case SPEECHDVM:
            _displaycursor(_GCURSOROFF);
            speech();
            break;
          case SCOPE:
            tb_string("scope",1);
            _displaycursor(_GCURSOROFF);
            mnuAtrib.fgBorder=14;
            scope();
            break;
          case TESTS:
            tb_string("tests",1);
            _displaycursor(_GCURSOROFF);
            videoconfig();
            break;
         }
  }
}

/*******************************************************************/
/* Initialisierungsmenu                                            */
/*******************************************************************/

void init()
{
for(;;)
  {
   mnuAtrib.fgBorder=5;				/* Roten Rand      */
   iInitCur = Menu(6,10,mnuInit,iInitCur );	/* Init Menu       */
   switch( iInitCur+8 )
     {
      case HAUPTMENU:
        return;
        break;
      case FILTER:
        mnuAtrib.fgBorder=0;
        Mcol(6,10,mnuInit,iInitCur+8);
        mnuAtrib.fgBorder=5;			/* Roten Rand      */
        filter();
        break;
      case FREQUENZ:
        mnuAtrib.fgBorder=0;
        Mcol(6,10,mnuInit,iInitCur+8);
        mnuAtrib.fgBorder=5;
        frequenz();
        break;
      case GAIN:
        mnuAtrib.fgBorder=0;
        Mcol(6,10,mnuInit,iInitCur+8);
        mnuAtrib.fgBorder=5;
        gain();
        break;
      case TAKTQUELLE:
        mnuAtrib.fgBorder=0;
        Mcol(6,10,mnuInit,iInitCur+8);
        mnuAtrib.fgBorder=5;
        taktquelle();
        break;
      case EINGANG:
        mnuAtrib.fgBorder=0;
        Mcol(6,10,mnuInit,iInitCur+8);
        mnuAtrib.fgBorder=5;
        eingang();
        if(iEingangCur==0)
         {
          mnuAtrib.fgBorder=5;
          filter();
          if(iFilterCur==0)
           {
            mnuAtrib.fgBorder=5;
            frequenz();
           }
         }
        else
         {
          mnuAtrib.fgBorder=5;
          polaritaet();
         }
         break;
      case POLARITAET:
        mnuAtrib.fgBorder=0;
        Mcol(6,10,mnuInit,iInitCur+8);
        mnuAtrib.fgBorder=5;
        polaritaet();
        break;
     case AUSGANG:
        mnuAtrib.fgBorder=0;
        Mcol(6,10,mnuInit,iInitCur+8);
        mnuAtrib.fgBorder=5;
        ausgang();
        break;
     case DISALL:
        mnuAtrib.fgBorder=0;
        Mcol(6,10,mnuInit,iInitCur+8);
        disall();
        break;
      }
  }  
}

/*******************************************************************/
/* Filter ein/ausschalten                                          */
/*******************************************************************/

void filter()
{
iFilterCur=Menu(3,46,mnuFilter,iFilterCur);
switch(iFilterCur)
   {
    case 0:
      REGISTER=REGISTER|0x40;
      break;
    case 1:
      REGISTER=REGISTER&0xbf;
      break;
   }
outpw(0x294,REGISTER+INGAIN+OUT);

mnuAtrib.fgBorder=0;				/* Schwarzen Rand  */
Mcol(3,46,mnuFilter,iFilterCur);		/* Einschalten     */
}

/*******************************************************************/
/* Filterfrequenz einstellen                                       */
/*******************************************************************/

void frequenz()
{
iFrequenzCur=Menu(12,66,mnuFrequenz,iFrequenzCur);
switch(iFrequenzCur)
  {
   case 0:
     FWERT=0x00;break;
   case 1:
     FWERT=0x2c;break;
   case 2:
     FWERT=0x3b;break;
   case 3:
     FWERT=0x48;break;
   case 4:
     FWERT=0x55;break;
   case 5:
     FWERT=0x5e;break;
   case 6:
     FWERT=100;break;
   case 7:
     FWERT=105;break;
   case 8:
     FWERT=109;break;
   case 9:
     FWERT=112;break;
   case 10:
     FWERT=114;break;
   case 11:
     FWERT=116;break;
   case 12:
     FWERT=118;break;
   case 13:
     FWERT=119;break;
   case 14:
     FWERT=120;break;
   case 15:
     FWERT=121;break;
   case 16:
     FWERT=122;break;
   case 17:
     FWERT=123;break;
   case 18:
     FWERT=124;break;
   case 19:
     FWERT=127;break;
   }
outpw(0x294,REGISTER+INGAIN+OUT);
outpw(0x292,FWERT);
outpw(0x294,REGISTER+INGAIN+OUT+0x20);
outpw(0x292,FWERT);

mnuAtrib.fgBorder=0;
Mcol(12,66,mnuFrequenz,iFrequenzCur);
}

/*******************************************************************/
/* Gain einstellen                                                 */
/*******************************************************************/

void gain()
{
iGainCur=Menu(13,46,mnuGain,iGainCur);
INGAIN=iGainCur;
outp(0x294,REGISTER+INGAIN+OUT);

mnuAtrib.fgBorder=0;
Mcol(13,46,mnuGain,iGainCur);
}

/*******************************************************************/
/* Taktquelle einstellen                                           */
/*******************************************************************/

void taktquelle()
{
iTaktquelleCur=Menu(22,46,mnuTaktquelle,iTaktquelleCur);
switch(iTaktquelleCur)
   {
    case 0:
      REGISTER=REGISTER&0x7f;
      break;
    case 1:
      REGISTER=REGISTER|0x80;
      break;
   }
outp(0x294,REGISTER+OUT+INGAIN);

mnuAtrib.fgBorder=0;
Mcol(22,46,mnuTaktquelle,iTaktquelleCur);
}

/*******************************************************************/
/* Eingang aktivieren                                              */
/*******************************************************************/

void eingang()
{
iEingangCur=Menu(5,28,mnuEingang,iEingangCur);
kanal=iEingangCur;
kanaladr=0x280+(iEingangCur*2);

mnuAtrib.fgBorder=0;
Mcol(5,28,mnuEingang,iEingangCur);
}

/*******************************************************************/
/* EingangspolaritÑt einstellen                                    */
/*******************************************************************/

void polaritaet()
{
iPolaritaetCur=Menu(13,28,mnuPolaritaet,iPolaritaetCur);
switch(iPolaritaetCur)
   {
    case 0:
      REGISTER=REGISTER&0xcf;
      break;
    case 1:
      REGISTER=REGISTER|0x10;
      break;
   }
outp(0x294,REGISTER+INGAIN+OUT);

mnuAtrib.fgBorder=0;
Mcol(13,28,mnuPolaritaet,iPolaritaetCur);
}

/*******************************************************************/
/* Ausgang A oder B wÑhlen                                         */
/*******************************************************************/

void ausgang()
{
iAusgangCur=Menu(19,28,mnuAusgang,iAusgangCur);
switch(iAusgangCur)
   {
    case 0:
      OUT=0x100;break;
    case 1:
      OUT=0x200;break;
    case 2:
      OUT=0x300;break;
   }
outp(0x294,REGISTER+GAIN+OUT);

mnuAtrib.fgBorder=0;
Mcol(19,28,mnuAusgang,iAusgangCur);
}

/*******************************************************************/
/* Alles anzeigen                                                  */
/*******************************************************************/

void disall()
{
Mcol(3,46,mnuFilter,iFilterCur);
Mcol(12,66,mnuFrequenz,iFrequenzCur);
Mcol(13,46,mnuGain,iGainCur);
Mcol(22,46,mnuTaktquelle,iTaktquelleCur);
Mcol(5,28,mnuEingang,iEingangCur);
Mcol(13,28,mnuPolaritaet,iPolaritaetCur);
Mcol(19,28,mnuAusgang,iAusgangCur);
}