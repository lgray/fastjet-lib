# gnuplot file


reset 
set dat sty li

set size square
set xrange [-5:5]
set ylabel 'P_{t,jet} / Area_{jet}'
set xlabel '{/Symbol h}'


plot 'scatter.res' u 2:($4/$5) t '' w p pt 7 ps 2 
replot 5.56 w l lt 0 t 'median' 




