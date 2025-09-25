//Delmotte Aurélien,Zhang Xiru
#include "ops.h"

//those aren't actually used, we instead use #defines as they are much faster, content-wise though they are the same.
#ifdef BAD
//ℂ²→ℂ
complex ccp(complex a,complex b){return (complex){a.re+b.re,a.im+b.im};}
complex ccm(complex a,complex b){return (complex){a.re-b.re,a.im-b.im};}
complex ccx(complex a,complex b){return (complex){a.re*b.re-a.im*b.im,a.re*b.im+a.im*b.re};}
complex ccd(complex a,complex b){
    double d = b.re*b.re+b.im*b.im;
    return (complex){(a.re*b.re-a.im*b.im)/d,(a.re*b.im+a.im*b.re)/d};
}
//ℂ×ℝ→ℂ
complex cdp(complex a,double b){return (complex){a.re+b,a.im};}
complex cdm(complex a,double b){return (complex){a.re-b,a.im};}
complex cdx(complex a,double b){return (complex){a.re*b,a.im*b};}
complex cdd(complex a,double b){return (complex){a.re/b,a.im/b};}
//ℝ×ℂ→ℂ    
complex dcp(double a,complex b){return (complex){a+b.re,b.im};}
complex dcm(double a,complex b){return (complex){a-b.re,0-b.im};}
complex dcx(double a,complex b){return (complex){a*b.re,a*b.im};}
complex dcd(double a,complex b){
    double d = b.re*b.re+b.im*b.im;
    return (complex){(a*b.re)/d,(a*b.im)/d};
}
//ℂ×ℤ→ℂ
complex cip(complex a,int b){return (complex){a.re+b,a.im};}
complex cim(complex a,int b){return (complex){a.re-b,a.im};}
complex cix(complex a,int b){return (complex){a.re*b,a.im*b};}
complex cid(complex a,int b){return (complex){a.re/b,a.im/b};}
//ℤ×ℂ→ℂ    
complex icp(int a,complex b){return (complex){a+b.re,b.im};}
complex icm(int a,complex b){return (complex){a-b.re,0-b.im};}
complex icx(int a,complex b){return (complex){a*b.re,a*b.im};}
complex icd(int a,complex b){
    double d = b.re*b.re+b.im*b.im;
    return (complex){(a*b.re)/d,(a*b.im)/d};
}

complex coj(complex a){return (complex){a.re,-a.im};}
#endif

complex nth_root(int n){
    complex nrt;
    double x = 2*M_PI/n;
    nrt.re = cos(x);
    nrt.im = sin(x);
    return nrt;
}
complex* itoc_arr(int* V,int n){
    complex* R = malloc(sizeof(complex)*n);
    for(int i=0;i<n;++i){
        R[i] = (complex){V[i],0};
    }
    return R;
}
