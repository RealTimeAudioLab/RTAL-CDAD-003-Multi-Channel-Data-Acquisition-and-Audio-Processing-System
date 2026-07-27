#include<stdio.h>
#include<conio.h>
#include<malloc.h>

int _huge *w1,*w2,*w3,*w4,*w5,*w6,*w7,*w8;
int i,j,y;

main()
{

w1=halloc(30000,sizeof(int));
w2=halloc(30000,sizeof(int));
w3=halloc(30000,sizeof(int));
w4=halloc(30000,sizeof(int));
w5=halloc(30000,sizeof(int));
w6=halloc(30000,sizeof(int));
w7=halloc(30000,sizeof(int));
w8=halloc(30000,sizeof(int));

outpw(0x334,0x100);		/* FILTER A */
outpw(0x332,125);		/* FREQUENZ FILTER A */

outpw(0x334,0x120); 		/* FILTER B */
outpw(0x332,125);		/* FREQUENZ FILTER B */

outpw(0x334,0x1d2);		/* OUT A; TAKT VOM TIMER 0; FILTER EIN; BIPOLAR */ 

outp(0x33f,0x3e);		/* TIMER 0 INITIALISIEREN */
outp(0x33c,0x05);		/* CLK 1,6 MHz FšR A/D WANDLER EINSTELLEN */
outp(0x33c,0x00);		/* CLK HIGH BYTE */

outp(0x33f,0x7e);		/* TIMER 1 INITIALISIEREN */
outp(0x33d,0xC8);		/* 70h FšR 14us C8h FšR 25us */
outp(0x33d,0x00);		/* HIGH BYTE */

_asm{cli}
printf("SAMPLING");
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  w1[i]=inpw(0x320);
  }  
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  w2[i]=inpw(0x320);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  w3[i]=inpw(0x320);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  w4[i]=inpw(0x320);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  w5[i]=inpw(0x320);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  w6[i]=inpw(0x320);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  w7[i]=inpw(0x320);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  w8[i]=inpw(0x320);
  }

printf("PLAYING");
for(j=0;j<1;j++)
{
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  inpw(0x320);
  outpw(0x336,w1[i]);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  inpw(0x320);
  outpw(0x336,w2[i]);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  inpw(0x320);
  outpw(0x336,w3[i]);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  inpw(0x320);
  outpw(0x336,w4[i]);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  inpw(0x320);
  outpw(0x336,w5[i]);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  inpw(0x320);
  outpw(0x336,w6[i]);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  inpw(0x320);
  outpw(0x336,w7[i]);
  }
for(i=0;i<30000;i++)
  {
  do
     y=inpw(0x338);
  while(y>=0x8000);
  inpw(0x320);
  outpw(0x336,w8[i]);
  }
}
_asm{sti}
hfree(w1);
hfree(w2);
hfree(w3);
hfree(w4);
hfree(w5);
hfree(w6);
hfree(w7);
hfree(w8);
}