#include<stdio.h>
#include<conio.h>
#include<malloc.h>

main()
{
int huge *array,y;
long int i;
if((array=(int huge*) halloc(200000,sizeof(int)))==0)
  fprintf(stderr,"AHUGE FEHLER !");
else
 {
  outpw(0x334,0x100);		/* FILTER A */
  outpw(0x332,125);		/* FREQUENZ FILTER A */
  outpw(0x334,0x120); 		/* FILTER B */
  outpw(0x332,125);		/* FREQUENZ FILTER B */
  outpw(0x334,0x1d1);		/* OUT A; TAKT VOM TIMER 0; FILTER EIN; BIPOLAR */ 
  outp(0x33f,0x3e);		/* TIMER 0 INITIALISIEREN */
  outp(0x33c,0x05);		/* CLK 1,6 MHz FšR A/D WANDLER EINSTELLEN */
  outp(0x33c,0x00);		/* CLK HIGH BYTE */
  outp(0x33f,0x7e);		/* TIMER 1 INITIALISIEREN */
  outp(0x33d,0xC8);		/* 70h FšR 14us C8h FšR 25us */
  outp(0x33d,0x00);		/* HIGH BYTE */

  _asm{cli}
  printf("SAMPLING");
  for(i=0;i<200000;i++)
    {
     do
       y=inpw(0x338);
     while(y>=0x8000);
     array[i]=inpw(0x320);
    }  
  printf("READY PRESS SPACE");
  getch();
  printf("PLAYING");
  for(i=0;i<200000;i++)
    {
     do
       y=inpw(0x338);
     while(y>=0x8000);
     inpw(0x320);
     outpw(0x336,array[i]);
    }
  _asm{sti}
  hfree(array);
 }
}