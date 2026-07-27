#include<stdio.h>
#include<graph.h>
#include<conio.h>
#include<math.h>
#define PI 3.14159

main()
{
struct videoconfig screen_size;
int M,N,n,w,y;
double m;
double real_sum,imag_sum,mean_sq_power,rms;
float max,x[128],abs;

outpw(0x294,0x00);      /* FITER A */
outpw(0x292,0xff);      /* FREQUENZ */
outpw(0x294,0x20);      /* FILTER B */
outpw(0x292,0xff);      /* FREQUENZ */
outp(0x28f,0x3e);       /* TIMER 0 */
outp(0x28c,0x05);       /* CLK 1,6 MHz */
outp(0x28c,0x00);       /* HIGH BYTE */
outp(0x28f,0x7e);       /* TIMER 1 */
outp(0x28d,0xc0);       /* WARTESCHLEIFE */
outp(0x28d,0x00);       /* HIGH BYTE */
outpw(0x294,0x10);      /* TAKT VOM TIMER 0; FILTER OFF; BIPOLAR; GAIN */
outp(0x29c,0x00);       /* NORMAL MODUS */

_settextposition(20,20);
printf("GEBE DIE ANZAHL DER DATEN EIN (MAX 128)");
scanf("%d",&N);

_setvideomode(_DEFAULTMODE);
_setvideomode(_HRES16COLOR);

_clearscreen(_GCLEARSCREEN);
_setbkcolor(_GRAY);
_getvideoconfig(&screen_size);
_setlogorg(screen_size.numxpixels/4,screen_size.numypixels/2);
_setcolor(1);
_moveto(0,20);
_lineto(280,20);
_moveto(0,20);
_lineto(0,-70);
for(n=305;n>=240;n-=30)
  {
   _moveto(n,20);
   _lineto(n,22);
  }
for(n=215;n>=95;n-=30)
  {
   _moveto(n,20);
   _lineto(n,22);
  }
for(n=60;n>=30;n-=30)
  {
   _moveto(n,20);
   _lineto(n,22);
  }
_moveto(0,-50);
_lineto(-3,-50);

_settextcolor(13);
_settextposition(2,29);
_outtext("FOURIER TRANSFORMATION");
_settextcolor(3);
_settextposition(10,7);
_outtext("R.M.S. Power");
_settextposition(7,15);
_outtext("MAX");
_settextposition(19,60);
_outtext("Frequency (Hz)");
_settextcolor(4);
_settextposition(17,59);
_outtext("20k");
_settextposition(17,40);
_outtext("10k");
_settextposition(17,24);
_outtext("2k");
_asm{cli}
do{
inpw(0x284);
for(n=0;n<=(N-1);n++)
  {
   do
     y=inpw(0x298);
   while(y>=0x8000);
   x[n]=(inpw(0x288)+0x800)&0xfff;
  }
_asm{sti}

for(m=1;m<(N/2);m++)
  {
   real_sum=0;
   imag_sum=0;
   for(n=0;n<=(N-1);n++)
     {
      real_sum+=x[n]*cos(2*PI*m*n/N)/N;
      imag_sum+=x[n]*sin(2*PI*m*n/N)/N;
     }
  rms=sqrt(pow(real_sum,2)+pow(imag_sum,2));

  _setcolor(0);
  _moveto(280*2*m/N,19);
  _lineto(280*2*m/N,-80);
  _setcolor(14);
  _moveto(280*2*m/N,19);
  _lineto(280*2*m/N,-rms/10+19);
  }
}
while(kbhit()==0);
_setvideomode(_DEFAULTMODE);
}
