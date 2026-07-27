#include<stdio.h>
#include<graph.h>
#include<conio.h>
#include<math.h>

main()
{
int a,b,c,d,e,f,g,h,i,j,l,y,k;
outpw(0x294,0x00);      /* FITER A */
outpw(0x292,0xff);      /* FREQUENZ */
outpw(0x294,0x20);      /* FILTER B */
outpw(0x292,0xff);      /* FREQUENZ */
outp(0x28f,0x3e);       /* TIMER 0 */
outp(0x28c,0x05);       /* CLK 1,6 MHz */
outp(0x28c,0x00);       /* HIGH BYTE */
outp(0x28f,0x7e);       /* TIMER 1 */
outp(0x28d,0x70);       /* WARTESCHLEIFE */
outp(0x28d,0x00);       /* HIGH BYTE */
outpw(0x294,0x10);      /* TAKT VOM TIMER 0; FILTER OFF; BIPOLAR; GAIN */
outp(0x29c,0x00);       /* NORMAL MODE */

printf("Kanal Åber Adresse 0 eingeben\nKanal fest Åber Register 1 eingeben: ");
k=getche()-0x30;
printf("\n\nKANAL 1\tKANAL2\tKANAL3\tKANAL4\tKANAL5\tKANAL6\n");
do
  {
   _asm{cli}

   if(k==1)
     outp(0x29c,0x80);
   g=inpw(0x280);

   do
     y=inpw(0x290);
   while(y<0x8000);

   do
     y=inpw(0x290);
   while(y>=0x8000);

   a=inpw(0x280);

   if(k==1)
     outp(0x29c,0x81);
   h=inpw(0x282);

   do
     y=inpw(0x290);
   while(y<0x8000);

   do
     y=inpw(0x290);
   while(y>=0x8000);

   b=inpw(0x282);

   if(k==1)
     outp(0x29c,0x82);
   i=inpw(0x284);

   do
     y=inpw(0x290);
   while(y<0x8000);

   do
     y=inpw(0x290);
   while(y>=0x8000);

   c=inpw(0x284);

   if(k==1)
     outp(0x29c,0x83);
   j=inpw(0x286);

   do
     y=inpw(0x290);
   while(y<0x8000);

   do
     y=inpw(0x290);
   while(y>=0x8000);

   d=inpw(0x286);
   
   if(k==1)
     outp(0x29c,0x84);
   k=inpw(0x288);

   do
     y=inpw(0x290);
   while(y<0x8000);

   do
     y=inpw(0x290);
   while(y>=0x8000);

   e=inpw(0x288);

   if(k==1)
     outp(0x29c,0x85);
   l=inpw(0x28a);

   do
     y=inpw(0x290);
   while(y<0x8000);

   do
     y=inpw(0x290);
   while(y>=0x8000);

   f=inpw(0x28a);

   _asm{sti}  
   printf("%x\t%x\t%x\t%x\t%x\t%x\r",a,b,c,d,e,f);
  }
while(kbhit()==0);
}
