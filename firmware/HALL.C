#include<stdio.h>
#include<graph.h>
#include<conio.h>
#include<math.h>
#define PI 3.14159

main()
{
float a,b,c,d,e,f,y,z;
unsigned int contents;

inpw(0x280);
do
  y=inpw(0x290);
while(y>=0x8000);
contents=inpw(0x280);
a=0.00392*contents;
y=0.5*z+a-0.03125*f;
outpw(0x296,(int)128*(1+0.516229*y));
f=e;
e=d;
d=c;
c=b;
b=a;
z=y;
}
