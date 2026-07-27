#include<stdio.h>
#include<conio.h>

int i,laenge,val,*input,x,y;
FILE *ez,*fopen();
char s1[2],s2[2],s3[2],sample[12000];
float z;
unsigned a,b,c;

main()
{
outpw(0x294,0x00);              /* FILTER A */
outpw(0x292,0x7f);              /* FREQUENZ FILTER A */
outpw(0x294,0x20);              /* FILTER B */
outpw(0x292,0x7f);              /* FREQUENZ FILTER B */
outpw(0x294,0x00);              /* TAKT VOM TIMER 0; FILTER EIN; BIPOLAR */ 
outp(0x28f,0x3e);               /* TIMER 0 INITIALISIEREN */
outp(0x28c,0x05);               /* CLK 1,6 MHz FšR A/D WANDLER EINSTELLEN */
				/* WERTE 05 BIS 0D ERLAUBT */
outp(0x28c,0x00);               /* CLK HIGH BYTE */
outp(0x28f,0x7e);               /* TIMER 1 INITIALISIEREN */
outp(0x28d,0x70);               /* 70h FšR 14us */
outp(0x28d,0x00);               /* HIGH BYTE */
outp(0x28c,0x00);               /* NORMAL MODE */
outp(0x29c,0x00);


laenge=12000;
y=inpw(0x288);                  /* KANAL  */
do 
   x=inpw(0x290);
while(x>=0x8000);
y=inpw(0x288);
do
   x=inpw(0x290);
while(x>=0x8000);
y=inpw(0x288);
if(y==0) 
  {
   a=0;b=0;c=0;
   goto ausgabe;
  }
z=(5.0/4096.0)*(float)y+0.01;
a=(int)z;                       /* a = Einer */
z=z*10.0;
b=(int)z-(a*10);
z=z*10.0;
c=(int)z-(a*100)-(b*10);

ausgabe:
printf("Die angelegte Spannung ber„gt: ");
printf("%d.%d%d VOLT\n",a,b,c);

chdir("..");
chdir("andi2");
chdir("voices");

       itoa(a,s1,10);
       ez=fopen(s1,"rb");
       fread(sample,sizeof(char),laenge,ez);
       OUT_LAUT(sample,laenge);
       fclose(ez);
       ez=fopen("10","rb");
       fread(sample,sizeof(char),laenge,ez);
       OUT_LAUT(sample,laenge);
       fclose(ez);
       itoa(b,s2,10);
       ez=fopen(s2,"rb");
       fread(sample,sizeof(char),laenge,ez);
       OUT_LAUT(sample,laenge);
       fclose(ez);
       itoa(c,s3,10);
       ez=fopen(s3,"rb");
       fread(sample,sizeof(char),laenge,ez);
       OUT_LAUT(sample,laenge);
       fclose(ez);
       ez=fopen("11","rb");
       fread(sample,sizeof(char),laenge,ez);
       OUT_LAUT(sample,laenge);
       fclose(ez);
       chdir("..");
       chdir("..");
       chdir("aneu");
}
