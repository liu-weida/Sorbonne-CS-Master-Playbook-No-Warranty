#include <math.h>
#include <stdio.h>
#include <cadna.h>

int main()
{
  printf("------------------------------------------\n");
  printf("|  Polynomial function of two variables  |\n");
  printf("------------------------------------------\n");

  cadna_init(-1);
  double_st x = 77617.;
  double_st y = 33096.;
  double_st res;

  res=333.75*y*y*y*y*y*y+x*x*(11.*x*x*y*y-y*y*y*y*y*y-121.*y*y*y*y-2.0)   
    +5.5*y*y*y*y*y*y*y*y+x/(2.*y);

  //printf("res=%.14e\n",res);
  printf("%s\n", strp(res));
  cadna_end();
  return 0;
}
