#include <stdio.h>
#include <math.h>
#include <cadna.h>
int main (){
  cadna_init(-1);
  double_st x,y,a;
  int i=0;
  printf("-----------------------------\n");
  printf("|  Logistic iteration       |\n");
  printf("-----------------------------\n");
  
  x=y=0.6;
  a=3.6;

  do {
         x=a*x*(1-x);	
         y=a*0.25-a*pow((x-0.5),2);
	 i=i+1;
         if (!(i%50)) printf("i=%3d x=%s y=%s\n",i,strp(x),strp(y));
      }  
  while (i < 200);
  printf("last iterate:\ni=%d x=%s y=%s\n",i,strp(x),strp(y));
  cadna_end();
  return 0;
}
