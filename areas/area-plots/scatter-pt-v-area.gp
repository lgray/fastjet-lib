# gnuplot file


reset 
set dat sty li

set size square
#set xrange [-5:5]
set ylabel 'P_{t,jet}'
set xlabel 'jet area'


set yrange [0:80]
set xrange [0:5]

#set title 'hard event + 10 min-bias (R=1)'

set label 1 "dijet event"    at 0.3,45
set label 2 "+ 10 minbias"  at 0.3,39
set label 3 "(Kt-alg, R=1)" at 0.3,31

set ytics 20

vetoedp(y,p) = abs(y)<5 ? p : -1

plot 'scatter.res' u 5:(vetoedp($2,$4)) t '' w p pt 7 ps 2 
replot 5.56*x w l lt 0 t 'median (pt/area)'

