//Delmotte Aurélien,Zhang Xiru
#include "alg.h"

void FFT_in(complex* restrict P, complex* restrict R, int n, complex omega, int st){
    if(n==1){
        R[0] = P[0];
        return;
    }
    //recursive calls
    complex om2 = ccx(omega,omega);
    FFT_in(P,R,n/2,om2,st*2);
    FFT_in(P+st,R+n/2,n/2,om2,st*2);
    //adjustments
    complex omi = {1,0};
    for(int i=0;i<n/2;i++){
        complex pom = ccx(R[i+n/2],omi);
        complex pe  = R[i];
        R[i]    = ccp(pe,pom);//addition
        R[n/2+i]= ccm(pe,pom);//substraction
        omi = ccx(omi,omega);
    }
}

complex* FFT(complex* restrict P, int n,int *l){                    
    //padding/copy, we could make it in-place for the purposes of our assignement, but I fear realloc isn't much faster
    int k;                        
    for(k=0;*l>(1<<k);k++);               
    *l = 1<<k;
    complex* V = malloc(sizeof(complex)* *l);
    for(int i=0;i<n;i++) V[i]=P[i];
    for(int i=n;i<*l;i++)V[i]=(complex){0,0};
    
    complex omega = nth_root(*l);
    complex* r = malloc(sizeof(complex)* *l);
    FFT_in(V,r,*l,omega,1);
    free(V); 
    return r;
}

complex* FFT_Inv(complex* restrict P, int n, int *l){
    for(int i=0;i<*l;++i) P[i] = coj(P[i]);//conjugate
    complex* r = FFT(P,n,l);
    for(int i=0;i<*l;++i) r[i] = cid(coj(r[i]),*l);//conjugate then divide

    for(int i=0;i<*l;++i) P[i] = coj(P[i]);//reverse the conjugate
    return r;
}

//order of V1 and V2 goes from smallest to largest exponant i.e. for a×x²+bx+c → (c,b,a)
int* FFT_Prod(int* restrict V1,int* restrict V2,int n, int m, int* l ){
    *l = n+m-1;
    //conversion to complex
    complex* CV1 = itoc_arr(V1,n);
    complex* CV2 = itoc_arr(V2,m);
    
    complex* fftV1 = FFT(CV1,n,l);
    complex* fftV2 = FFT(CV2,m,l);
    
    complex* fftR  = malloc(sizeof(complex) * *l);
    for (int i = 0; i < *l; i++) fftR[i] = ccx(fftV1[i] , fftV2[i]);//multiplication
    
    complex* ifftR = FFT_Inv(fftR,*l,l);
    //reconvert to int
    int* res = malloc(sizeof(int)* *l);
    for (int i = 0; i < *l; i++) res[i] = round(ifftR[i].re);

    free(CV1);
    free(CV2);
    free(fftR);
    free(ifftR);
    free(fftV1);
    free(fftV2);
    return res;
}
int* Naive_Prod(int* restrict V1,int* restrict V2, int n, int m, int* l){
    *l = n+m-1;
    int* r = calloc(*l,sizeof(int));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            r[i+j]+=V1[i]*V2[j];
        }
    }
    return r;
}

