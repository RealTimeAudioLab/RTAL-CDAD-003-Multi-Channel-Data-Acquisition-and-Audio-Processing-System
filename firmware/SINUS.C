#include<stdio.h>
#include<conio.h>
#include<math.h>
#define PI 3.1472

int val,*input,j,y;
float werte[100];
long i;
main()
{
outpw(0x294,0x300);             /* FILTER A */
outpw(0x292,0x7f);              /* FREQUENZ FILTER A */
outpw(0x294,0x320);             /* FILTER B */
outpw(0x292,0x7f);              /* FREQUENZ FILTER B */
outpw(0x294,0x3d0);             /* TAKT VOM TIMER 0; FILTER EIN; BIPOLAR */ 
outp(0x28f,0x3e);               /* TIMER 0 INITIALISIEREN */
outp(0x28c,0x05);               /* CLK 1,6 MHz FšR A/D WANDLER EINSTELLEN */
outp(0x28c,0x00);               /* CLK HIGH BYTE */
outp(0x28f,0x7e);               /* TIMER 1 INITIALISIEREN */
outp(0x28d,0x70);               /* 70h FšR 14us */
outp(0x28d,0x00);               /* HIGH BYTE */
outp(0x28c,0x00);               /* NORMAL MODE */
outp(0x29c,0x00);

for(i=0;i<100;i++)
  werte[i]=sin(2*PI*i/100)*2048;
printf("EIN SINUS WIRD AM DA-WANDLER AUSGEGEBEN");
_asm{cli}
for(i=0;i<5000;i++)
  {
   for(j=0;j<100;j++)
     {
      outpw(0x296,(int)werte[j]);
     }
  }
_asm{sti}
}
