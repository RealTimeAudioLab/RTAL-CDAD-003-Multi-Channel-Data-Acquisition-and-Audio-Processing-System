/*******************************************************************/
/* Datum anzeigen                                                  */
/*******************************************************************/

void showdate()
{
struct dosdate_t date;
char list[20],s1[6];

if(!_setvideomode(_MAXRESMODE))exit(1);
_getvideoconfig(&vc);
_setviewport(0,0,640,480);
_clearscreen(_GCLEARSCREEN);

strcpy(list,face[4]);
strcat(list,"h500w65b");
if(_setfont(list)<0)
  {
   printf("Fehler, Font nicht setzbar !");
   exit(1);
  }

_dos_getdate(&date);
_moveto(10,10);
itoa(date.day,s1,10);
if (date.day<10) _outgtext("0");
_outgtext(s1);
_outgtext(":");
itoa(date.month,s1,10);
if(date.month<10) _outgtext("0");
_outgtext(s1);
_outgtext(":");
itoa(date.year,s1,10);
_outgtext(s1);
getch();
_setvideomode(_DEFAULTMODE);
}

/*******************************************************************/
/* Uhrzeit anzeigen                                                */
/*******************************************************************/

void showtime()
{
struct dostime_t time;
char list[20],s1[3];
int y,i=1,j=0;
  
if(!_setvideomode(_ERESCOLOR))exit(0);
_getvideoconfig(&vc);
_setviewport(0,0,640,350);
_clearscreen(_GCLEARSCREEN);
strcpy(list,face[4]);
strcat(list,"h400w50b");
if(_setfont(list)<0)
 {
  _setvideomode(_DEFAULTMODE);
  printf("Keine Fonts setzbar 1!");
  exit(0);
 }
_setcolor(11);
do
  {
  i=i^1;
  if(_setvisualpage(i)==-1);            /* Diese Seite ist sichtbar */
  j=j^1;

  if(_setactivepage(j)==-1);            /* Auf diese Seite soll geschrieben werden */
  _clearscreen(_GCLEARSCREEN);
  _moveto(60,0);
  _dos_gettime(&time);
  if(time.hour<10)
    _outgtext(" ");
  itoa(time.hour,s1,10);
  _outgtext(s1);
  _outgtext(" : ");
  if(time.minute<10)
    _outgtext(" ");
  itoa(time.minute,s1,10);
  _outgtext(s1);
  _outgtext(" : ");
  if(time.second<10)
    _outgtext(" ");
  itoa(time.second,s1,10);
  _outgtext(s1);
  y=time.second;
  do 
    {
     _dos_gettime(&time);
    }
  while(time.second==y);
  }
while(kbhit()==0);
_setvisualpage(0);
_setactivepage(0);
_setvideomode(_DEFAULTMODE);
}

/*******************************************************************/
/* Diskrete Fourier Transformation                                 */
/*******************************************************************/

void dft()
{
struct videoconfig screen_size;
int M,N,n,w,y;
double m;
double real_sum,imag_sum,mean_sq_power,rms;
float max,x[128],abs;

outp(0x28d,0xc0);               /* Warteschleife */
outp(0x28d,0x00);

_settextposition(20,20);
printf("GEBE DIE ANZAHL DER DATEN EIN (MAX 128)");
scanf("%d",&N);
if(N>128) N=128;

_setvideomode(_HRES16COLOR);
_clearscreen(_GCLEARSCREEN);
_setbkcolor(_GRAY);
_getvideoconfig(&screen_size);
_setlogorg(screen_size.numxpixels/4,screen_size.numypixels/2);
_setcolor(3);
_moveto(0,20);
_lineto(320,20);
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

_settextcolor(13);
_settextposition(2,29);
_outtext("FOURIER TRANSFORMATION");
_settextcolor(3);
_settextposition(7,7);
_outtext("R.M.S. POWER");
_settextposition(19,60);
_outtext("FREQUENCY (Hz)");
_settextcolor(4);
_settextposition(17,59);
_outtext("20k");
_settextposition(17,40);
_outtext("10k");
_settextposition(17,24);
_outtext("2k");
do{
_asm{cli}
inpw(kanaladr);
for(n=0;n<=(N-1);n++)
  {
   do
     y=inpw(0x298);
   while(y>=0x8000);
   x[n]=(inpw(kanaladr)+0x800)&0xfff;
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
  _moveto(320*2*(int)m/N,19);
  _lineto(320*2*(int)m/N,-80);
  _setcolor(14);
  _moveto(320*2*(int)m/N,19);
  _lineto(320*2*(int)m/N,-(int)rms/10+19);
  }
}
while(kbhit()==0);
outp(0x28d,0x70);               /* 70h fÅr 14us reinitialisieren */
outp(0x28d,0x00);
_setvideomode(_DEFAULTMODE);
}

/*******************************************************************/
/* Sprechendes Voltmeter                                           */
/*******************************************************************/

speech()
{
int y;
unsigned a,b,c;
float z;
FILE *ez,*fopen();
char s1[2],s2[2],s3[2];

y=inpw(kanaladr);
do{} 
while(inpw(0x290)>=0x8000);
y=inpw(kanaladr);
do{}
while(inpw(0x290)>=0x8000);
y=inpw(kanaladr);

if(y!=0) 
  {
   z=(5.0/4096.0)*(float)y+0.01;
   a=(int)z;                    /* a = Einer */
   z=z*10.0;
   b=(int)z-(a*10);             /* b = Zehntel */
   z=z*10.0;
   c=(int)z-(a*100)-(b*10);     /* c = Hundertstel */
  }
else 
  {
   a=0;b=0;c=0;
  }

printf("    Die am Kanal %d, Adresse %x angelegte Spannung berÑgt: ",
	kanal+1,kanaladr);
printf("%d.%d%d VOLT\n",a,b,c);

       itoa(a,s1,10);
       if(tb_string(s1,1) eq RC_FAIL)
	{
	 tb_terminate();
	 return(98);
	}
       if(tb_string("KOMMA",1) eq RC_FAIL)
	{
	 tb_terminate();
	 return(98);
	}
       itoa(b,s2,10);
       if(tb_string(s2,1) eq RC_FAIL)
	{
	 tb_terminate();
	 return(98);
	}
       itoa(c,s3,10);
       if(tb_string(s3,1) eq RC_FAIL)
	{
	 tb_terminate();
	 return(98);
	}
       if(tb_string("VOLT",1) eq RC_FAIL)
	{
	 tb_terminate();
	 return(98);
	}
}

/*******************************************************************/
/* Rechnertests                                                    */
/*******************************************************************/

void videoconfig()
{
_setvideomode(_MAXRESMODE);
_getvideoconfig(&vc);
printf("\n   MAXRESMODE\n\n");
printf("Horizontale Punkte: %d\nVertikale Punkte: %d\n",
	vc.numxpixels,vc.numypixels);
printf("Anzahl der Textzeilen: %d\n",vc.numtextrows);
printf("Anzahl der Spalten im Textmodus: %d\n",vc.numtextcols);
printf("Anzahl der verfÅgbaren Farben: %d\n",vc.numcolors);
printf("Anzahl der Bits per Pixel: %d\n",vc.bitsperpixel);
printf("Anzahl der verfÅgbaren Videopages: %d\n",vc.numvideopages);
printf("Aktueller Video-Modus: %d\n",vc.mode);
switch(vc.adapter)
   {
    case _CGA: printf("CGA adapter\n");
	       break;
    case _EGA: printf("EGA adapter\n");
	       break;
    case _MDPA:printf("MDPA adapter\n");
	       break;
    case _MCGA:printf("MCGA adapter\n");
	       break;
    case _VGA: printf("VGA adapter\n");
	       break;
   }
switch(vc.monitor)
   {
    case _ANALOG:   printf("Analog monitor\n");
		    break;
    case _COLOR:    printf("Color monitor\n");
		    break;
    case _ENHCOLOR: printf("Enhanced color monitor\n");
		    break;
    case _MONO:     printf("Monochrome moniter\n");
		    break;
   }
printf("VerfÅgbarer Videospeicher: %d KB\n",vc.memory);
getch();
_setvideomode(_DEFAULTMODE);
}

/*******************************************************************/
/* Oszilloskop                                                     */ 
/*******************************************************************/

void scope()
{
register int scopeadkanal;
unsigned int i,j,s,o,x,y,iScopeCur=3,iTimebaseCur=0,afaktor,bfaktor;
unsigned afalt,bfalt;
char s1[4],s2[4],f,g,h,list[20];
unsigned huge *werte1;
unsigned huge *z;

if(!(werte1=halloc(1000,sizeof(unsigned))))
  {
   printf("\nFEHLER SPEICHERZUORDNUNG SCOPE1 !");
   getch();
   exit(0);
  }
if(!(z=halloc(1000,sizeof(unsigned))))
  {
   printf("\nFEHLER SPEICHERZUORDNUNG SCOPE2 !");
   getch();
   exit(0);
  }

if(!_setvideomode(_MAXRESMODE))exit(1);
_setviewport(0,0,640,480);
_clearscreen(_GCLEARSCREEN);
strcpy(list,face[1]);
strcat(list,"h10w10b");
if(_setfont(list)<0)
  {
   printf("Fehler, Font nicht setzbar !");
   exit(1);
  }

scopeadkanal=kanaladr;
_setcolor(2);
_moveto(120,1);                         /* Umrandung */
_lineto(638,1);
_lineto(638,478);
_lineto(120,478);
_lineto(120,1);
for(i=10;i<=200;i++)
  {
   _moveto(1,i);
   _lineto(110,i);
  }
for(i=280;i<=468;i++)
  {
   _moveto(1,i);
   _lineto(110,i);
 }

for(i=40;i<=440;i+=40)                  /* Horizontale Linien */
  {
   _moveto(140,i);
   _lineto(620,i);
  }
for(i=140;i<=620;i+=40)                 /* Vertikale Linien */
  {
   _moveto(i,40);
   _lineto(i,440);
  }
for(i=160;i<=600;i+=40)                 /* Horizontale Zwischenlinien */
  {
   _moveto(i,238);
   _lineto(i,242);
  }
for(i=48;i<=440;i+=8)                   /* Vertikale Zwischenlinien */
  {
   _moveto(377,i);
   _lineto(383,i);
  }  
_setcolor(1);
_moveto(14,20);
_outgtext("AMPLITUDE");
_moveto(19,290);
_outgtext("TIMEBASE");

weiter:
   iScopeCur=Menu(7,7,mnuScope,iScopeCur);
   switch(iScopeCur)
	 {
	  case QUIT:
	    free(z);free(werte1);
	    _setvideomode(_DEFAULTMODE);
	    return;
	    break;
	  case 1:
	    afaktor=2;
	    bfaktor=-783;
	    break;
	  case 2:
	    afaktor=4;
	    bfaktor=-271;
	    break;
	  case 3:
	    afaktor=9;
	    bfaktor=13;
	    break;
	  case 4:
	    afaktor=18;
	    bfaktor=127;
	    break;
	  case 5:
	    afaktor=36;
	    bfaktor=184;
	    break;
	 }
   iTimebaseCur=Menu(24,7,mnuTimebase,iTimebaseCur);
   switch(iTimebaseCur)
	 {
	  case 0:
	    break;
	  case 1:
	    break;
	  case 2:
	    break;
	  case 3:
	    break;
	  case 4:
	    break;
	  case 5:
	    break;
	 }

_setcolor(14);
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
	   mov cx,1000
	   mov dx,scopeadkanal
	   in ax,dx
      l1:  mov dx,290h
      l0:  in ax,dx
	   and ax,8000h
	   jnz l0
	   mov dx,scopeadkanal
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
i=0;
do
  {
   i++;
   if(i>749) goto weit1;
  }
while(z[i]<0x800||z[i+1]>0x800);

for(j=0;j<1000-i;j++)
  z[j]=z[j+i];

weit1:
   for(i=0;i<480;i++)
     {
      if(_getpixel(i+140,bfalt+werte1[i]/afalt)==14)
	{
	 _setcolor(0);
	 _setpixel(i+140,bfalt+werte1[i]/afalt);
	}
      if(_getpixel(i+140,bfaktor+z[i]/afaktor)!=2)
	{
	 _setcolor(14);
	 _setpixel(i+140,bfaktor+z[i]/afaktor);
	}
     }
   for(i=0;i<480;i++)
      werte1[i]=z[i];
   afalt=afaktor;
   bfalt=bfaktor;
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
	 goto raus;
	 }
      }
   while(z[i]<0x7f9||z[i+1]>0x805);
   itoa((i*13)-(i/2),s1,10);
   raus:
   _settextposition(29,40);
   _outtext("PERIODENDAUER = ");
   _outtext(s1);
   _outtext(" us   ");
   goto weiter;
  }
_setvideomode(_DEFAULTMODE);
hfree(z);
hfree(werte1);
}
