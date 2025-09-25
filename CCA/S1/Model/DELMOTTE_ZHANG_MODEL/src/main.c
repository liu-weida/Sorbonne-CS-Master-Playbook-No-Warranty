//Delmotte Aurélien,Zhang Xiru
#include "alg.h"
#include <time.h>
#include <sys/ioctl.h>
#define MINL  50000 
#define MAXL  200000
#define MAX   10000
#define MINC -50
#define MAXC  50
#define ITER  5
//random array generation for tests.
int* randarr(int* l,int minl,int maxl){
    *l = minl;
    if(maxl>minl) *l = rand()%(maxl-minl)+minl;
    int* r = malloc(sizeof(int)* *l);
    for(int i=0;i<*l;i++){
        r[i] = rand()%(MAXC-MINC)+MINC;
    }
    return r;
}
//only ever used in the debug mode
void print_pol(int* pol, int l){
    printf("( ");
    for(int i=0;i<l;i++){
        printf("%dx^%d + ",pol[i],i);
    }
    printf("\b\b)");
}
//progress bar for when the output isn't stdout
void print_pb(double i,double max){
    struct winsize w;                    
    ioctl(0, TIOCGWINSZ, &w);               
    int ww =  w.ws_col-12;
    double prog = i/max;
    int progb   = prog*ww;
    printf("\r[");
    for (int j = 0; j < ww; ++j) {
        printf(j < progb ? "#" : "-");
    } 
    printf("] %.2f%%", prog * 100);
    fflush(stdout);
}

double test(int* (*f)(int* restrict,int* restrict,int,int,int*),int seed,int iter,int minl1,int maxl1,int minl2,int maxl2){
    int *n   = malloc(sizeof(int )*iter);
    int *m   = malloc(sizeof(int )*iter);
    int l;
    int **V1 = malloc(sizeof(int*)*iter);
    int **V2 = malloc(sizeof(int*)*iter);
    int *r;
    //pregenerate random arrays
    srand(seed);
    for(int i=0;i<iter;i++) V1[i] = randarr(n+i,minl1,maxl1);
    srand(seed);
    for(int i=0;i<iter;i++) V2[i] = randarr(m+i,minl2,maxl2);

    clock_t start_time = clock();

    for(int i=0;i<iter;i++){
        D(
            print_pol(V1[i],n[i]);
            printf("×");
            print_pol(V2[i],m[i]);
            putchar('=');
        )  
        r = f(V1[i],V2[i],n[i],m[i],&l);
        D(
            print_pol(r,l);  
            putchar('\n');
        )
        free(r);
    }

    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    for(int i=0;i<iter;i++){
        free(V1[i]);
        free(V2[i]);
    }
    free(V1);
    free(V2);
    free(m);
    free(n);
    return elapsed_time; 
}
int main(int argc,char* argv[]){
    int step= 1;
    int min = 1;
    int max = MAX;
    int iter= ITER;
    int ste2= 1;
    int min2= -1;
    int max2= 0;
    FILE *f = stdout;
    int G   = 0;
    int G2  = 0;
    srand(time(NULL));
    if(argc>4){ 
        iter= atoi(argv[1]);
        min = atoi(argv[2]);
        max = atoi(argv[3]);
        if(argv[4][0]=='G'){
            G      = 1;
            step   = atoi(argv[4]+1);
        }else step = atoi(argv[4]);
    }
    if(argc>7){
        min2= atoi(argv[5]);
        max2= atoi(argv[6]);
        if(argv[7][0]=='G'){               
            G2     = 1;
            ste2   = atoi(argv[7]+1);
        }else ste2 = atoi(argv[7]);
    }
    if(argc==2) f = fopen(argv[1],"w");
    if(argc==6) f = fopen(argv[5],"w");
    if(argc>8)  f = fopen(argv[8],"w");
    int a,b;
    //solely for progress bar, barbaric way to count the expected number of iterations
    for(a=min2;a<max2;a+=G2?ceil((double)(ste2*a)/max2):ste2) 
        for(b=min;b<max;b+=G?ceil((double)(step*b)/max):step);
    fprintf(f,"#n\tm\tnaive\t\tfft\n");
    for(int j=min2;j<max2;j+=G2?ceil((double)(ste2*j)/max2):ste2){
        for(int i=min;i<max;i+=G?ceil((double)(step*i)/max):step){
            int tj = (max2==0)?i:j;
            int rd = rand();
            fprintf(f,"%d\t%d\t%.10f\t",tj,i,test(Naive_Prod,rd,iter,tj,tj,i,i)/iter);
            fprintf(f,"%.10f\n", test(FFT_Prod,rd,iter,tj,tj,i,i)/iter);
            if(f!=stdout) print_pb((((((double)i-min)/step)/(((double)b-min)/step)*
                                       (G2?ceil((double)(ste2*j)/max2):ste2)+j   -min2)/ste2),
                                                              (((double)a-min2)/ste2));
        }
        fprintf(f,"\n");
    }
    if(f!=stdout) {
        print_pb(max,max);
        putchar('\n');
        fclose(f);
    }
    return 0;
}
