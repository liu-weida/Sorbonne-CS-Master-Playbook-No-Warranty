//Delmotte Aurélien,Zhang Xiru
#ifndef OPS_H
#define OPS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef DBG
#define D(x) {x}
#else
#define D(x)
#endif
#define Dprintf(...) D(fprintf(stderr,__VA_ARGS__);)

#define G2d(X,x,y,n) ((X)[(y)*(n)+(x)])

typedef struct {
    double re;
    double im;
} complex;

#ifndef BAD
//ℂ²→ℂ
#define ccp(a, b) ((complex){(a).re + (b).re, (a).im + (b).im})
#define ccm(a, b) ((complex){(a).re - (b).re, (a).im - (b).im})
#define ccx(a, b) ((complex){(a).re * (b).re - (a).im * (b).im, (a).re * (b).im + (a).im * (b).re})
#define ccd(a, b) ({ double d = (b).re * (b).re + (b).im * (b).im;\
                  ((complex){((a).re * (b).re - (a).im * (b).im) / d, ((a).re * (b).im + (a).im * (b).re) / d}); })
//ℂ×ℝ→ℂ
#define cdp(a, b) ((complex){(a).re + (b), (a).im})
#define cdm(a, b) ((complex){(a).re - (b), (a).im})
#define cdx(a, b) ((complex){(a).re * (b), (a).im * (b)})
#define cdd(a, b) ((complex){(a).re / (b), (a).im / (b)})

//ℝ×ℂ→ℂ
#define dcp(a, b) ((complex){(a) + (b).re, (b).im})
#define dcm(a, b) ((complex){(a) - (b).re, 0 - (b).im})
#define dcx(a, b) ((complex){(a) * (b).re, (a) * (b).im})
#define dcd(a, b) ({ double d = (b).re * (b).re + (b).im * (b).im; ((complex){(a) / d, (a) / d}); })

//ℂ×ℤ→ℂ
#define cip(a, b) ((complex){(a).re + (b), (a).im})
#define cim(a, b) ((complex){(a).re - (b), (a).im})
#define cix(a, b) ((complex){(a).re * (b), (a).im * (b)})
#define cid(a, b) ((complex){(a).re / (b), (a).im / (b)})

//ℤ×ℂ→ℂ
#define icp(a, b) ((complex){(a) + (b).re, (b).im})
#define icm(a, b) ((complex){(a) - (b).re, 0 - (b).im})
#define icx(a, b) ((complex){(a) * (b).re, (a) * (b).im})
#define icd(a, b) ({ double d = (b).re * (b).re + (b).im * (b).im; ((complex){(a) / d, (a) / d}); })

#define coj(a)    ((complex){(a).re,-(a).im})
#else
//ℂ²→ℂ
complex ccp(complex a,complex b);
complex ccm(complex a,complex b);
complex ccx(complex a,complex b);
complex ccd(complex a,complex b);
//ℂ×ℝ→ℂ
complex cdp(complex a,double b);
complex cdm(complex a,double b);
complex cdx(complex a,double b);
complex cdd(complex a,double b);
//ℝ×ℂ→ℂ
complex dcp(double a,complex b);
complex dcm(double a,complex b);
complex dcx(double a,complex b);
complex dcd(double a,complex b);
//ℂ×ℤ→ℂ
complex cip(complex a,int b);
complex cim(complex a,int b);
complex cix(complex a,int b);
complex cid(complex a,int b);
//ℤ×ℂ→ℂ
complex icp(int a,complex b);
complex icm(int a,complex b);
complex icx(int a,complex b);
complex icd(int a,complex b);

complex coj(complex a);
#endif

complex  nth_root(int n);
complex* itoc_arr(int* V,int n);
#endif
