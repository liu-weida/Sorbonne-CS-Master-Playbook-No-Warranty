//Delmotte Aurélien,Zhang Xiru
#ifndef ALG_H
#define ALG_H
#include "ops.h"

complex* FFT(complex* restrict P, int n, int *l);
complex* FFT_Inv(complex* restrict P, int n, int *l);

int* FFT_Prod(int* restrict V1, int* restrict V2, int n, int m, int *l);
int* Naive_Prod(int* restrict V1,int* restrict V2, int n, int m, int *l);

#endif
