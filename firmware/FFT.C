#include<stdio.h>
#include<graph.h>
#include<conio.h>
#include<math.h>
#define PI 3.14159

int N,a,c,d,f,g,h,j,contents,x[1024];
float ar[1024],ai[1024],window[1024];
double b,e,k,l,co,si,mean_sq_power,average,n,ti,rms;
int m,p,q,r,s,t,u,w,y,z,max;
main()
{
struct videoconfig screen_size;

outpw(0x294,0x00);      /* FITER A */
outpw(0x292,0xff);      /* FREQUENZ */
outpw(0x294,0x20);      /* FILTER B */
outpw(0x292,0xff);      /* FREQUENZ */
outp(0x28f,0x3e);       /* TIMER 0 */
outp(0x28c,0x05);       /* CLK 1,6 MHz */
outp(0x28c,0x00);       /* HIGH BYTE */
outp(0x28f,0x7e);       /* TIMER 1 */
outp(0x28d,0x70);       /* 70 h FšR 14us */
outp(0x28d,0x00);       /* HIGH BYTE */
outpw(0x294,0x90);      /* TAKT VOM TIMER 0; FILTER OFF; BIPOLAR; GAIN */
outp(0x29c,0x00);       /* NORMAL MODUS */

_settextposition(20,20);
printf("GEBE DIE ANZAHL DER DATEN EIN (MAX 1024)");
scanf("%d",&N);
n=log10(N)/log10(2);

_setvideomode(_DEFAULTMODE);
_setvideomode(_HRES16COLOR);
_clearscreen(_GCLEARSCREEN);
_setbkcolor(_GRAY);
_getvideoconfig(&screen_size);
_setlogorg(screen_size.numxpixels/4,screen_size.numypixels/2);
_moveto(0,0);
_lineto(320,0);
_moveto(0,0);
_lineto(0,-90);
_settextcolor(13);
_settextposition(2,27);
_outtext("FAST FOURIER TRANSFORMATION");
_settextcolor(3);
_settextposition(4,6);
_outtext("R.M.S. Power");
_settextposition(14,50);
_outtext("Frequency (Hz)");
_asm{cli}
inpw(0x300);
do
 {
  for(z=1;z<=N;z++)
    {
     do
       y=inpw(0x290);
     while(y>=0x8000);
     x[z]=(inpw(0x280)+0x800)&0xfff;
    }
_asm{sti}

for(z=1;z<=N;z++)
  x[z]=x[z]/16;

  average=0;
  for(z=1;z<=N;z++)
    {
     average+=x[z]/(double)N;
     ai[z]=0;
    }
  for(z=1;z<=N;z++)
    {
     window[z]=0.5*(1-cos(2*PI*(z-1)/N));
     ar[z]=5*(x[z]-average)*window[z]/255;
    }
  a=N;
  b=2*PI/N;
  for(c=1;c<=n;c++)
    {  
     d=a;
     a=a/2;
     e=0;
     for(f=1;f<=a;f++)
       {
	co=cos(e);
	si=sin(e); 
	e=e+b;
	u=1;
	for(g=d;g<=N;g=u*d)
	  {
	   u++;
	   h=g-d+f;
	   j=h+a;
	   k=ar[h]-ar[j];
	   l=ai[h]-ai[j];
	   ar[h]=ar[h]+ar[j];
	   ai[h]=ai[h]+ai[j];
	   ar[j]=co*k+si*l;
	   ai[j]=co*l-si*k;
	  }
       }
     b=2*b;
    }
  m=1;
  p=N/2;
  q=N-1;
  for(r=1;z<=q;r++)
    {
     if(r>(m-0.1))goto label1;
     k=ar[m];
     l=ai[m];
     ar[m]=ar[r];
     ai[m]=ai[r];
     ar[r]=k;
     ai[r]=l;
  label1:s=p;
  label3:if(s>(m-0.1)) goto label2;
     m=m-s;
     s=s/2;
     goto label3;
  label2:m=m+s;
    }
_clearscreen(_GCLEARSCREEN);
  for(w=1;w<=N;w++)
    {
     mean_sq_power=pow(ar[w]/N,2)+pow(ai[w]/N,2);
     rms=sqrt(mean_sq_power);
     _setcolor(14);
     ti=320*(double)(w-1)/N;
     _moveto(ti,0);
     _lineto(ti,-100*rms);
    }
 }
while(kbhit()==0);
_setvideomode(_DEFAULTMODE);
}
