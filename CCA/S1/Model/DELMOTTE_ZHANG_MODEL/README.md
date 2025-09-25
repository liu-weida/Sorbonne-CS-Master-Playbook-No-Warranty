# Projet Model
by Xiru Zhang and Aurélien Delmotte

## Compiling
To compile, simply run:
```
make
```
or alteratively to use the debug flag run:
```
make CFLAGS="-Wall -O3 -lm -DDBG"
```

##  USAGE
with default parameters:
```
./main [FILE]
```
with specific parameters and both polynomials of same size:
```
./main <ITER> <n MIN> <n MAX> <n STEP> [FILE] 
```
with specific parameters and polynomials of differing sizes:
```
./main <ITER> <n MIN> <n MAX> <n STEP> <m MIN> <m MAX> <m STEP> [FILE] 
```
- **FILE** is the output ( average time spent for a given size pair ) if unspecified it is set to stdout.
- **ITER** is the number of times over which to average each computation.
- **n MIN** is the minimum size of either the first polynomial or both if m MIN unspecified.
- **n MAX** is the maximum size of either the first polynomial or both if m MAX unspecified.
- **n STEP** is the interval between sizes, it can be appended at the start with a G to indicate the interval be proportional to n, the specified number then becomes the max interval also indicates m STEP if it is unspecified.
- **m MIN** is the minimum size of either the first polynomial.
- **m MAX** is the maximum size of either the first polynomial.
- **m STEP** is the interval between sizes, it can be appended at the start with a G to indicate the interval be proportional to m, the specified number then becomes the max interval.

## Examples
- ./main 50 1 50000 G200 ./out/u.txt
- ./main 50 1 40000 G1000 1 40000 G1000 ./out/o.txt

## plots

also included in ./out/ is the gnuplot file we used, it expects an o.txt file and an u.txt file, the o.txt file should be generated with the same arguments as specified in the second exempled ( apart from ITER )
