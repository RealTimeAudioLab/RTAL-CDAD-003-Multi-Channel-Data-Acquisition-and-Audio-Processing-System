#include<stdio.h>
#include<conio.h>
#include<malloc.h>

int _huge *w1;
int i,j,y,anzahl=100;
FILE *ez,*fopen();

main()
{

w1=halloc(30000,sizeof(int));

outpw(0x314,0x90);		/* FILTER A */
outpw(0x312,0xff);		/* FREQUENZ FILTER A */

outpw(0x314,0xb0); 		/* FILTER B */
outpw(0x312,0xff);		/* FREQUENZ FILTER B */

outpw(0x314,0x51);		/* TAKT VOM TIMER 0; FILTER EIN; BIPOLAR */ 

outp(0x31f,0x3e);		/* TIMER 0 INITIALISIEREN */
outp(0x31c,0x05);		/* CLK 1,6 MHz FšR A/D WANDLER EINSTELLEN */
outp(0x31c,0x00);		/* CLK HIGH BYTE */

outp(0x31f,0x7e);		/* TIMER 1 INITIALISIEREN */
outp(0x31d,0x70);		/* 70h FšR 14us */
outp(0x31d,0x00);		/* HIGH BYTE */

printf("SAMPLING");
if(!(ez=fopen("muster.ref","wb")))
  {
   printf("KANN DATEI NICHT ™FFNEN !");
   getch();
   exit(0);
  }
for(j=0;j<anzahl;j++)
  {
   for(i=0;i<30000;i++)
     {
      do
        y=inpw(0x318);
      while(y>=0x8000);
      w1[i]=inpw(0x300);
     }   
   if((fwrite(w1,sizeof(int),30000,ez))!=30000)
     {
      printf("SCHREIBFEHLER !");
      getch();
      exit(0);
     }
  }
fclose(ez);

printf("PLAYING");
if(!(ez=fopen("muster.ref","rb")))
  {
   printf("KANN DATEI NICHT ™FFNEN !");
   getch();
   exit(0);
  }
for(j=0;j<anzahl;j++)
  {
   if((fread(w1,sizeof(int),30000,ez))!=30000)
     {
      printf("LESEFEHLER !");
      getch();
      exit(0);
     }
   for(i=0;i<30000;i++)
     {
      do
        y=inpw(0x318);
      while(y>=0x8000);
      inpw(0x300);
      outpw(0x316,w1[i]);
     }
  }
fclose(ez);
hfree(w1);
}
