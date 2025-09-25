#Delmotte Aurélien, Zhang Xiru
set term pdfcairo size 8,6 font "Arial,12" enhanced color solid

set samples    500,500
set isosamples 500,500
set hidden3d
set pm3d noborder
set logscale x
set logscale y
set logscale z

set format x "%g"
set format y "%g"
set format z "%g"

set xlabel "n"
set ylabel "Δt"
set log cb

stats "u.txt" using 1 nooutput
min_x = STATS_min              
max_x = STATS_max              

set xrange [min_x:max_x]

set output 'plot_diag.pdf'
plot "u.txt" using 1:3 with lines title "naive", \
     "u.txt" using 1:4 with lines title "fft"

unset xrange
unset yrange

stats "o.txt" using 1 nooutput
min_x = STATS_min
max_x = STATS_max

set xrange [min_x:max_x]

set output '/dev/null'
plot "u.txt" using 1:3 with lines title "naive n=m" dashtype 4 linecolor rgb "black", \
     "u.txt" using 1:4 with lines title "fft n=m" linecolor rgb "black"

replot "<awk -v p=0 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:3 with lines title "naive m = 1" dashtype 4 linecolor rgb "red", \
       "<awk -v p=0 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:4 with lines title "fft m = 1" linecolor rgb "red"

replot "<awk -v p=66 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:3 with lines title "naive m = 99" dashtype 4 linecolor rgb "green", \
       "<awk -v p=66 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:4 with lines title "fft m = 99" linecolor rgb "green"

replot "<awk -v p=154 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:3 with lines title "naive m = 1012" dashtype 4 linecolor rgb "purple", \
       "<awk -v p=154 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:4 with lines title "fft m = 1012" linecolor rgb "purple"

replot "<awk -v p=246 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:3 with lines title "naive m = 9963" dashtype 4 linecolor rgb "magenta", \
       "<awk -v p=246 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:4 with lines title "fft m = 9963" linecolor rgb "magenta"

set output 'plot_2dslices.pdf'
replot "<awk -v p=302 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:3 with lines title "naive m = 39770" dashtype 4 linecolor rgb "blue", \
       "<awk -v p=302 'n==p; NF{n++} !NF{n=0}' o.txt" using 1:4 with lines title "fft m = 39770" linecolor rgb "blue"

stats "o.txt" using 2 nooutput
min_y = STATS_min
max_y = STATS_max

set yrange [min_y:max_y]

set xlabel "n"
set ylabel "m"
set zlabel "Δt"

set output 'plot_3dnaive.pdf'
splot "o.txt" using 1:2:3 with pm3d title "naive"

set output 'plot_3dnaiveTH.pdf'
splot x*y with pm3d

set output 'plot_3dfft.pdf'
splot "o.txt" using 1:2:4 with pm3d title "fft"

set output 'plot_3dfftTH.pdf'
splot 2**ceil(log(x+y)/log(2))*ceil(log(x+y)/log(2)) with pm3d

set output 'inter.pdf'
splot "o.txt" using 1:2:3:(abs($3-$4)<=0.1*($3>$4?$3:$4) ? $3 : 1/0 ) with pm3d title "Intersection"

set output 'min.pdf'
splot "o.txt" using 1:2:($3<$4 ? $3 : $4 ):($3<$4 ? $3 : $4 ) with pm3d title "Min"

set output 'max.pdf'
splot "o.txt" using 1:2:($3>$4 ? $3 : $4 ):($3>$4 ? $3 : $4 ) with pm3d title "Max"


set pm3d map

set output 'map_3dnaive.pdf'
splot "o.txt" using 1:2:3 with pm3d title "naive"

set output 'map_3dnaiveTH.pdf'
splot x*y with pm3d

set output 'map_3dfft.pdf'
splot "o.txt" using 1:2:4 with pm3d title "fft"

set output 'map_3dfftTH.pdf'
splot 2**ceil(log(x+y)/log(2))*ceil(log(x+y)/log(2)) with pm3d
