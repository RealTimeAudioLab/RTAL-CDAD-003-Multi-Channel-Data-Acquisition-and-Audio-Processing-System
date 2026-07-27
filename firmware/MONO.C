#include<stdio.h>
#include<conio.h>

void menue();

int val,*input;
unsigned FWERT=0x7f,REGISTER=0xd0,GAIN=0x2,TAKT=0x05,OUT=0x300,
	 ARATE=0x0070,ARATEL=0x70,ARATEH=0x00;
char s1;
char *gwerte[]={"1","2","4","8","16","32","64","128"};
char *fwerte[]={"1.000","1.011","1.023","1.035","1.047","1.060","1.073",
		"1.087","1.100","1.114","1.129","1.143","1.158","1.174",
		"1.190","1.206","1.223","1.241","1.259","1.277","1.296",
		"1.315","1.335","1.356","1.378","1.400","1.422","1.446",
		"1.470","1.495","1.521","1.548","1.576","1.605","1.635",
		"1.666","1.699","1.732","1.767","1.804","1.842","1.881",
		"1.923","1.966","2.011","2.058","2.108","2.160","2.215",
		"2.272","2.333","2.397","2.464","2.536","2.611","2.692",
		"2.777","2.868","2.966","3.070","3.181","3.301","3.431",
		"3.571","3.571","3.620","3.671","3.723","3.777","3.832",
		"3.888","3.947","4.007","4.069","4.133","4.200","4.268",
		"4.338","4.411","4.487","4.565","4.646","4.729","4.816",
		"4.906","5.000","5.097","5.198","5.303","5.412","5.526",
		"5.645","5.769","5.898","6.034","6.176","6.325","6.481",
		"6.645","6.818","7.008","7.191","7.394","7.608","7.835",
		"8.076","8.333","8.606","8.898","9.210","9.545","9.905",
		"10.294","10.714","11.170","11.666","12.209","12.804",
		"13.461","14.189","15.000","15.909","16.935","18.103",
		"19.444","21.000","22.826","25.000"};

main()
{
register int eingang=0x280;

outpw(0x294,REGISTER+OUT);      /* FILTER A */
outpw(0x292,FWERT);             /* FREQUENZ FILTER A */
outpw(0x294,REGISTER+0x20+OUT); /* FILTER B */
outpw(0x292,FWERT);             /* FREQUENZ FILTER B */
outpw(0x294,REGISTER+OUT+GAIN); /* TAKT VOM TIMER 0; FILTER EIN; BIPOLAR */ 
outp(0x28f,0x3e);               /* TIMER 0 INITIALISIEREN */
outp(0x28c,TAKT);               /* CLK 1,6 MHz FšR A/D WANDLER EINSTELLEN */
				/* WERTE 05 BIS 0D ERLAUBT */
outp(0x28c,0x00);               /* CLK HIGH BYTE */
outp(0x28f,0x7c);               /* TIMER 1 INITIALISIEREN */
outp(0x28d,ARATEL);             /* 70h FšR 14us */
outp(0x28d,ARATEH);             /* HIGH BYTE */
outp(0x28f,0xbc);
outp(0x28e,0x70);
outp(0x28e,0x00);
outp(0x29c,0x00);               /* NORMAL MODE */

menue(eingang);
start:
s1=inp(0x21);                   /* Interruptcontroller1 8259 */
outp(0x21,0xfd);                /* Nur Tastatur */
_asm
     {
      mov dx,eingang
      in ax,dx
l1:   
      mov dx,298h
lo:   in ax,dx
      and ax,8000h
      jnz lo
      mov dx,eingang
      in ax,dx
      mov dx,29ah
      out dx,ax
      mov dx,296h
      out dx,ax
      mov ah,01h
      int 16h
      jnz raus 
      jmp l1
      raus:
     }
outp(0x21,s1);                          /* Controller 1 reinitialisieren */
val=getch();
input=&val;
switch(val)
    {
     case'1':   printf("\nFILTER EIN/AUS\n");
		if((REGISTER&0x40)==0) 
		  {
		   REGISTER+=0x40;
		   outpw(0x294,REGISTER+GAIN+OUT);
		   printf("FILTER EIN\n");
		  }
		else
		  {
		   REGISTER-=0x40;
		   outpw(0x294,REGISTER+GAIN+OUT);
		   printf("FILTER AUS\n");
		  }
		goto start;
		break;
     case'2':   printf("\nEINGANG A/B\n");
		if(eingang==0x280)
		  {
		   eingang=0x282;
		   printf("EINGANG B AKTIV");
		  }
		else
		  {
		   eingang=0x280;
		   printf("EINGANG A AKTIV");
		  }
		goto start;
		break;
     case'3':   printf("\nABTASTRATE ERH™HEN\n");
		if(ARATEL!=0x08||ARATEH!=0x00)
		  {
		   ARATE=ARATE-8;
		   ARATEL=ARATE&0xff;
		   ARATEH=(ARATE&0xff00)>>8;
		   outp(0x28f,0x7e);
		   outp(0x28d,ARATEL);
		   outp(0x28d,ARATEH);
		   printf("NEUE ABTASTRATE : %d us\n",ARATE/8);
		  }
		else printf("MAXIMALE ABTASTRATE ERREICHT !\n");
		goto start;
		break;
     case'4':   printf("\nABTASTRATE SENKEN\n");
		ARATE=ARATE+8;
		ARATEL=ARATE&0xff;
		ARATEH=(ARATE&0xff00)>>8;
		outp(0x28f,0x7e);
		outp(0x28d,ARATEL);
		outp(0x28d,ARATEH);
		printf("NEUE ABTASTRATE : %d us\n",ARATE/8);
		goto start;
		break;
     case '+':  printf("\nFILTERFRQUENZ ERH™HEN\n");
		if(FWERT!=0x7f)
		  {
		   if((REGISTER&0x40)==0) REGISTER+=0x40;    
		   printf("FILTER EIN\n");
		   FWERT+=1;
		   outpw(0x294,REGISTER+GAIN+OUT);
		   outpw(0x292,FWERT);          /* FREQU FILTER A +1 */
		   outpw(0x294,REGISTER+0x20+GAIN+OUT); /* FILTER B */
		   outpw(0x292,FWERT);          /* FREQU FILTER B +1 */
		   printf("NEUE FILTERFREQUENZ : %s kHz\n",fwerte[FWERT]);
		  }
		else printf("MAXIMALER WERT ERREICHT !\n");
		goto start;
		break;
     case '-':  printf("\nFILTERFREQUENZ SENKEN\n");
		if(FWERT!=0)
		  {
		   if((REGISTER&0x40)==0) REGISTER+=0x40;
		   printf("FILTER EIN\n");
		   FWERT-=1;
		   outpw(0x294,REGISTER+GAIN+OUT);
		   outpw(0x292,FWERT);
		   outpw(0x294,REGISTER+0x20+GAIN+OUT);
		   outpw(0x292,FWERT);
		   printf("NEUE FILTERFREQUENZ : %s kHz\n",fwerte[FWERT]);
		  }
		else printf("MINIMALER WERT ERREICHT !\n");
		goto start;
		break;
     case 'a':  printf("\nTAKTQUELLE ŽNDERN\n");
		if(REGISTER >= 0x90)
		  {
		   REGISTER-=0x80;
		   outpw(0x294,REGISTER+GAIN+OUT);
		   printf("FESTE TAKTQUELLE 1,44 MHz\n");
		  }
		else 
		  {
		   REGISTER+=0x80;
		   outpw(0x294,REGISTER+GAIN+OUT);
		   printf("TAKTQUELLE IST DER 8254\n");
		  }
		goto start;
		break;
     case 'p':  printf("\nGAIN\n");
		if(GAIN<0x07) 
		  {
		   GAIN+=1;
		   outp(0x294,REGISTER+GAIN+OUT);
		   printf("GAIN FAKTOR ERH™HEN AUF : %s\n",gwerte[GAIN]);
		  }
		else printf("GAIN MAXIMUM ERREICHT !\n");
		goto start;
		break;
     case 'm':  printf("\nGAIN\n");
		if(GAIN!=0)
		  {
		   GAIN-=1;
		   outpw(0x294,REGISTER+GAIN+OUT);
		   printf("GAIN FAKTOR SENKEN AUF : %s\n",gwerte[GAIN]);
		  }
		else printf("GAIN MINIMUM ERREICHT !\n");
		goto start;
		break;
     case 'h':  menue(eingang);
		goto start;
		break;
     case 0x1b: exit(0);
		break;
     default:   goto start;
		break;
    }
}

/*****************************************************************************/
void menue(unsigned eingang)
{
printf("\nMENUE : \n\n");
printf("TASTE <1> FšR FILTER EIN/AUS, z.Z. ");
if((REGISTER&0x40)==0) printf("FILTER AUS\n");
else printf("FILTER EIN\n");
printf("TASTE <2> FšR EINGANG A/B, z.Z. ");
if(eingang==0x280) printf("EINGANG A\n");
else printf("EINGANG B\n");
printf("TASTE <3> FšR ABTASTRATE ERH™HEN, z.Z. %d us\n",ARATE/8);
printf("TASTE <4> FšR ABTASTRATE SENKEN, z.Z. %d us\n",ARATE/8);
printf("TASTE <+> FšR FILTERFREQUENZ ERH™HEN, z.Z. %s kHz\n",fwerte[FWERT]);
printf("TASTE <-> FšR FILTERFREQUENZ SENKEN, z.Z. %s kHz\n",fwerte[FWERT]);
printf("TASTE <p> FšR GAIN ERH™HUNG, z.Z. FAKTOR %s\n",gwerte[GAIN]);
printf("TASTE <m> FšR GAIN SENKEN, z.Z. FAKTOR %s\n",gwerte[GAIN]);
printf("TASTE <a> FšR TAKTQUELLE AD-WANDLER ŽNDERN, z.Z. ");
if(REGISTER<0x90) printf("FEST 1,44 MHz\n");
else printf("der 8254\n");
printf("TASTE <h> FšR MENUE\n");
printf("TASTE <ESC> FšR ENDE\n");
}
