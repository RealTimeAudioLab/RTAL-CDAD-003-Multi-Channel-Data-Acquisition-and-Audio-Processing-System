#include<stdio.h>
#include<graph.h>
#include<conio.h>
#include<dos.h>
#include<malloc.h>
#define NFONTS 6

int werte1[2000];

static unsigned char *face[NFONTS]=
{
 "t'courier'","t'helv'","t'tms rmn'","t'modern'","t'script'","t'roman'"
};
static unsigned char list[20];

main()
{
unsigned int i,j,s,o,x,y;
unsigned huge *z;
struct videoconfig vc;
register int adkanal1;
char s1[4],s2[4],f,g,h;

if(!(z=halloc(2000,sizeof(unsigned))))
  {
   printf("SPEICHERFEHLER");
   getch();
   exit(0);
  }
if(_registerfonts("*.FON")<0)
 {
  printf("Keine Fonts ladbar !");
  exit(0);
 }

outpw(0x294,0x00);      /* FITER A */
outpw(0x292,0x7f);      /* FREQUENZ */
outpw(0x294,0x20);      /* FILTER B */
outpw(0x292,0x7f);      /* FREQUENZ */
outpw(0x294,0xd0);      /* TAKT=8254; BIPOLAREINGANG; GAIN=0 */
outp(0x28f,0x3e);       /* TIMER 0 */
outp(0x28c,0x05);       /* CLK 1,6 MHz */
outp(0x28c,0x00);       /* HIGH BYTE */
outp(0x28f,0x7e);       /* TIMER 1 */
outp(0x28d,0x70);       /* 70 h FšR 14us */
outp(0x28d,0x00);       /* HIGH BYTE */
outp(0x29c,0x00);       /* NORMAL MODE */

if(!_setvideomode(_MAXRESMODE))exit(0);
_getvideoconfig(&vc);
_setviewport(0,0,640,480);
_clearscreen(_GCLEARSCREEN);
strcpy(list,face[5]);
strcat(list,"h40w48b");
if(_setfont(list)<0)
 {
  _setvideomode(_DEFAULTMODE);
  printf("Keine Fonts setzbar 1!");
  exit(0);
 }
_setcolor(11);
_moveto(60,60);
_outgtext("PC - SCOPE");
_settextcolor(3);
_setcolor(10);
_moveto(0,0);
_lineto(639,0);
_moveto(639,0);
_lineto(639,479); 
_moveto(639,479);
_lineto(0,479);
_moveto(0,479);
_lineto(0,0);
_setcolor(9);
_moveto(2,2);
_lineto(637,2);
_moveto(637,2);
_lineto(637,477); 
_moveto(637,477);
_lineto(2,477);
_moveto(2,477);
_lineto(2,2);

strcpy(list,face[3]);
strcat(list,"h26w16b");
if(_setfont(list)<0)
 {
  _setvideomode(_DEFAULTMODE);
  printf("Keine Fonts setzbar 2!");
  exit(0);
 }
_setcolor(13);
do
  {
    _moveto(50,200);
    _outgtext("BITTE DEN KANAL EINGEBEN (1-6) : ");
    _settextcursor(0x0007);
    f=getch();
  }
while(f<49||f>54);
itoa(f-48,s1,10);
_outgtext(s1);
adkanal1=0x27e+((f-48)*2);
_setcolor(9);
_moveto(120,300);
_outgtext("MAXIMA ANZEIGEN ? (J/N)");
h=getch();

_clearscreen(_GCLEARSCREEN);
_setcolor(12);
_moveto(0,0);                           /* Umrandung */
_lineto(639,0);
_moveto(639,0);
_lineto(639,479);
_moveto(639,479);
_lineto(0,479);
_moveto(0,479);
_lineto(0,0);

_setcolor(2);
for(i=40;i<=440;i+=40)                  /* Horizontale Linien */
  {
   _moveto(40,i);
   _lineto(600,i);
  }
for(i=40;i<=600;i+=40)                  /* Vertikale Linien */
  {
   _moveto(i,40);
   _lineto(i,440);
  }
for(i=60;i<=600;i+=40)                  /* Horizontale Zwischenlinien */
  {
   _moveto(i,238);
   _lineto(i,242);
  }
for(i=48;i<=440;i+=8)                   /* Vertikale Zwischenlinien */
  {
   _moveto(318,i);
   _lineto(322,i);
  }

_setcolor(14);
weiter:
do
 {
      s = FP_SEG(z);
      o = FP_OFF(z);
      _asm
	  {
	   cli
	   mov ax,ds
	   push ax
	   mov ds,s
	   mov bx,o
	   mov cx,2000
	   mov dx,adkanal1
	   in ax,dx
      l1:  mov dx,290h
      l0:  in ax,dx
	   and ax,8000h
	   jnz l0
	   mov dx,adkanal1
	   in ax,dx
	   add ax,800h
	   and ax,0fffh
	   mov ds:[bx],ax
	   inc bx
	   inc bx
	   loop l1
	   pop ax
	   mov ds,ax
	   sti
	  }
i=0;
do
  {
   i++;
   if(i>749) goto weit1;
  }
while(z[i]<0x800||z[i+1]>0x800);

for(j=0;j<2000;j++)
  z[j]=z[j+i];

weit1:
   for(i=0;i<560;i++)
     {
      if(_getpixel(i+40,36+werte1[i]/10)==14)
	{
	 _setcolor(0);
	 _setpixel(i+40,36+werte1[i]/10);
	}
      if(_getpixel(i+40,36+z[i]/10)!=2)
	{
	 _setcolor(14);
	 _setpixel(i+40,36+z[i]/10);
	 if(h==106)
	  {
	   if(z[i]==0||z[i]==0xfff)
	    {
	     _settextposition(2,2);
	     _settextcolor(4);
	     _outtext("OVER");
	    }
	   else
	    {
	     _settextposition(2,2);
	     _settextcolor(0);
	     _outtext("    ");
	    }
	  }
	}
     }
   for(i=0;i<560;i++)
      werte1[i]=z[i];
 }
while(kbhit()==0);
if(getch()!=0x1b)
  {
   i=0;
   do
     {
      i++;
      if(i>799)
	{
	 i=0;
	 itoa(i,s1,10);
	 goto raus;
	 }
      }
while(z[i]<0x7f9||z[i+1]>0x805);
itoa((i*13)-(i/2),s1,10);
raus:
_settextposition(29,6);
_settextcolor(4);
_outtext("VERT. AMPL 0,5 V/cm");
_settextposition(29,57);
_outtext("TIME BASE 0,5 ms/cm");
_settextposition(29,30);
_outtext("PERIODENDAUER = ");
_outtext(s1);
_outtext(" us   ");
getch();
goto weiter;
  }
hfree(z);
_setvideomode(_DEFAULTMODE);
_unregisterfonts();
}
