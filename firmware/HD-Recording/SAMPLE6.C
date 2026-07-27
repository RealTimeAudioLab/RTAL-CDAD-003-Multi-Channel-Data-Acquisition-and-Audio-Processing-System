#include<stdio.h>
#include<conio.h>
#include<malloc.h>

main()
{
int huge *array;
long int i;
if((array=(int huge *) halloc(100000,sizeof(int))) == 0)
  fprintf(stderr,"AHUGE : FEHLER");
else
 {
  for(i=0;i<100000;i++)
    array[i]=i/10;
  display_array(array,i);
  hfree(array);
 }
}

display_array(int huge array[],long int size)
{
 long int i;
for(i=0;i<size;i+=1000)
 {
  printf("%d\n",array[i]);
  getch();
 }
}