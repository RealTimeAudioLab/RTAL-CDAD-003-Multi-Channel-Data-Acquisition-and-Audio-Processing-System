#include<stdio.h>
#include<graph.h>
#include<conio.h>
#include<dos.h>
#include<malloc.h>

int werte1[2000];
int werte2[2000];

main()
{
unsigned int i,j,s,o,x,y,*z,*z1;
struct videoconfig vc;
register int adkanal1;
register int adkanal2;
char s1[4],s2[4],f,g,h;

z=malloc(2000);
z1=malloc(2000);

outpw(0x294,0x00);      /* FITER A */
outpw(0x292,0x7f);      /* FREQUENZ */
outpw(0x294,0x20);      /* FILTER B */
outpw(0x292,0x7f);      /* FREQUENZ */
outpw(0x294,0x0d0);     /* TAKT; BIPOLAR; GAIN */
outp(0x28f,0x3e);       /* TIMER 0 */
outp(0x28c,0x05);       /* CLK 1,6 MHz */
outp(0x28c,0x00);       /* HIGH BYTE */
outp(0x28f,0x7e);       /* TIMER 1 */
outp(0x28d,0x70);       /* 70 h FšR 14us */
outp(0x28d,0x00);       /* HIGH BYTE */
outp(0x29c,0x00);       /* NORMAL MODE */

if(!_setvideomode(_ERESCOLOR))exit(1);
_getvideoconfig(&vc);
_setviewport(0,0,640,350);
_clearscreen(_GCLEARSCREEN);

_settextcolor(4);
_settextposition(4,31);
_outtext("PC - SCOPE by A.H.");
_settextcolor(3);
_setcolor(10);
_moveto(0,0);
_lineto(639,0);
_moveto(639,0);
_lineto(639,340); 
_moveto(639,340);
_lineto(0,340);
_moveto(0,340);
_lineto(0,0);
_setcolor(9);
_moveto(2,2);
_lineto(637,2);
_moveto(637,2);
_lineto(637,338); 
_moveto(637,338);
_lineto(2,338);
_moveto(2,338);
_lineto(2,2);

do
  {
    _settextposition(8,23);
    _outtext("BITTE ERSTEN KANAL EINGEBEN (1-6) : ");
    f=getch();
  }
while(f<49||f>54);
itoa(f-48,s1,10);
_outtext(s1);
adkanal1=0x27e+((f-48)*2);
do
  {
    _settextposition(12,23);
    _outtext("BITTE ZWEITEN KANAL EINGEBEN (1-6) : ");
    g=getch();
  }
while(g<49||g>54);
itoa(g-48,s2,10);
_outtext(s2);
adkanal2=0x27e+((g-48)*2);
_settextposition(16,23);
_outtext("MAXIMA ANZEIGEN ? (J/N)");
h=getch();

_clearscreen(_GCLEARSCREEN);
_settextcolor(4);
_settextposition(2,20);
_outtext("TIME BASE: 0,3 ms/cm    ");
_outtext("VERT. AMPL: 1 V/cm");
_settextcolor(3);
_settextposition(4,18);
_outtext("KANAL ");
_outtext(s1);
_settextposition(4,59);
_outtext("KANAL ");
_outtext(s2);
_settextcolor(10);
_settextposition(24,18);
_outtext("PRESS SPACE TO FREEZE PICTURE OR <ESC> FOR QUITT");
_setcolor(5);
_moveto(0,0);
_lineto(639,0);
_moveto(639,0);
_lineto(639,340);
_moveto(639,340);
_lineto(0,340);
_moveto(0,340);
_lineto(0,0);
_setcolor(2);
for(i=70;i<=270;i+=20)
  {
   _moveto(40,i);
   _lineto(280,i);
  }
for(i=70;i<=270;i+=20)
  {
   _moveto(370,i);
   _lineto(610,i);
  }
_moveto(160,70);
_lineto(160,270);
_moveto(490,70);
_lineto(490,270);
for(i=52;i<280;i+=24)
  {
   _moveto(i,168);
   _lineto(i,172);
  }
for(i=382;i<610;i+=24)
  {
   _moveto(i,168);
   _lineto(i,172);
  }
for(i=40;i<=280;i+=24)
  { 
   _moveto(i,90);
   _lineto(i,250);
  }
for(i=370;i<=610;i+=24)
  {
   _moveto(i,90);
   _lineto(i,250);
  }
_moveto(158,260);
_lineto(162,260);
_moveto(158,80);
_lineto(162,80);
_moveto(488,260);
_lineto(492,260);
_moveto(488,80);
_lineto(492,80);
for(i=94;i<250;i+=4)
  {
   _moveto(158,i);
   _lineto(162,i);
  }
for(i=94;i<250;i+=4)
  {
   _moveto(488,i);
   _lineto(492,i);
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

      s = FP_SEG(z1);
      o = FP_OFF(z1);
      _asm
	  {
	   cli
	   mov ax,ds
	   push ax
	   mov ds,s
	   mov bx,o
	   mov cx,2000
	   mov dx,adkanal2
	   in ax,dx
      l3:  mov dx,290h
      l2:  in ax,dx
	   and ax,8000h
	   jnz l2
	   mov dx,adkanal2
	   in ax,dx
	   add ax,800h
	   and ax,0fffh
	   mov ds:[bx],ax
	   inc bx
	   inc bx
	   loop l3
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
   for(i=0;i<240;i++)
     {
      if(_getpixel(i+40,119+werte1[i]/40)==14)
	{
	 _setcolor(0);
	 _setpixel(i+40,119+werte1[i]/40);
	}
      if(_getpixel(i+40,119+z[i]/40)!=2)
	{
	 _setcolor(14);
	 _setpixel(i+40,119+z[i]/40);
	 if(h==106)
	   {
	    if(z[i]==0||z[i]==0xfff) 
	      {
	       _settextposition(4,6);
	       _settextcolor(4);
	       _outtext("OVER");
	      }
	    else
	      {
	       _settextposition(4,6);
	       _settextcolor(0);
	       _outtext("    ");
	      }
	    }
	}
     }
   for(i=0;i<250;i++)
      werte1[i]=z[i];

i=0;
do
  {
   i++;
   if(i>749) goto weit2;
  }
while(z1[i]<0x800||z1[i+1]>0x800);

for(j=0;j<2000;j++)
  z1[j]=z1[j+i];

weit2:
   for(i=0;i<240;i++)
     {
      if(_getpixel(i+370,119+werte2[i]/40)==14)
	{
	 _setcolor(0);
	 _setpixel(i+370,119+werte2[i]/40);
	}
      if(_getpixel(i+370,119+z1[i]/40)!=2)
	{
	 _setcolor(14);
	 _setpixel(i+370,119+z1[i]/40);
	 if(h==106)
	   {
	    if(z1[i]==0||z1[i]==0xfff)
	      {
	       _settextposition(4,47);
	       _settextcolor(4);
	       _outtext("OVER");
	      }
	    else
	      {
	       _settextposition(4,47);
	       _settextcolor(0);
	       _outtext("    ");
	      }
	    }
	}
     }

   for(i=0;i<250;i++)
      werte2[i]=z1[i];
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
	  goto raus1;
	 }
     }
   while(z1[i]<0x7f9||z1[i+1]>0x805);
   itoa((i*13)-(i/2),s1,10);
   raus1:
   _settextcolor(6);
   _settextposition(22,51);
   _outtext("PERIODENDAUER = ");
   _outtext(s1);
   _outtext(" us  ");

   i=0;
   do
     {
      i++;
      if(i>799) 
	 {
	  i=0;
	  itoa(i,s1,10);
	  goto raus2;
	 }
     }
   while(z[i]<0x7f9||z[i+1]>0x805);
   itoa((i*13)-(i/2),s1,10);
   raus2:
   _settextcolor(6);
   _settextposition(22,10);
   _outtext("PERIODENDAUER = ");
   _outtext(s1);
   _outtext(" us    ");
   getch();
   goto weiter;
  }
free(z);
free(z1);
_setvideomode(_DEFAULTMODE);
}
