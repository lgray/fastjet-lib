# gnuplot file

reset
set dat sty li

# obtained from a fit:
#  fit [-5:5] c-b*x**2 'hydjet-r0.4-cell0.00025.res' u 2:($4/$5) via b,c
#b=6.86
#c=338
b=6.95
c=339
f(x) = c - b*x**2


set key spacing 1.5

set label 1 'k_t algorithm' at graph 0.05,0.93 front

set grid front
set xrange [-5:5]
set yrange [-25:50]

set xlabel 'y'
set ylabel 'Corrected Jet P_t' 0.5

plot 'hydjet-r0.4-cell0.00025.res' u 2:($4-$5*f($2)) w p lt 2pt 7 ps 2 t 'R = 0.4'
`gnupr hydjet-scatter-r0.4-corr.eps col`


plot 'hydjet-r0.3-cell0.00025.res' u 2:($4-$5*f($2)) w p lt 2pt 7 ps 2 t 'R = 0.3'
`gnupr hydjet-scatter-r0.3-corr.eps col`

plot 'hydjet-r0.225-cell0.00025.res' u 2:($4-$5*f($2)) w p lt 2pt 7 ps 2 t 'R = 0.225' 
`gnupr hydjet-scatter-r0.225-corr.eps col`


set ylabel 'Jet P_t'
set yrange [-30:200]

plot 'hydjet-r0.4-cell0.00025.res' u 2:($4) w p pt 7 ps 2 t 'uncorrected R = 0.4'
replot 'hydjet-r0.4-cell0.00025.res' u 2:($4-$5*f($2)) w p pt 7 ps 2 t 'corrected R = 0.4'
`gnupr hydjet-scatter-r0.4-both.eps col`


plot 'hydjet-r0.3-cell0.00025.res' u 2:($4) w p pt 7 ps 2 t 'uncorrected R = 0.3'
replot 'hydjet-r0.3-cell0.00025.res' u 2:($4-$5*f($2)) w p pt 7 ps 2 t 'corrected R = 0.3'
`gnupr hydjet-scatter-r0.3-both.eps col`

plot 'hydjet-r0.225-cell0.00025.res' u 2:($4) w p pt 7 ps 2 t 'uncorrected R = 0.225'
replot 'hydjet-r0.225-cell0.00025.res' u 2:($4-$5*f($2)) w p pt 7 ps 2 t 'corrected R = 0.225'
`gnupr hydjet-scatter-r0.225-both.eps col`


#======================================================================
set label 1 'Cam/Aachen algo' at graph 0.05,0.93 front

set grid front
set xrange [-5:5]
set yrange [-25:50]

set xlabel '{/Symbol h}'
set ylabel 'Corrected Jet P_t' 0.5

plot 'hydjet-cam-r0.4-cell0.00025.res' u 2:($4-$5*f($2)) w p lt 2pt 7 ps 2 t 'R = 0.4'
`gnupr hydjet-scatter-cam-r0.4-corr.eps col`


plot 'hydjet-cam-r0.3-cell0.00025.res' u 2:($4-$5*f($2)) w p lt 2pt 7 ps 2 t 'R = 0.3'
`gnupr hydjet-scatter-cam-r0.3-corr.eps col`

plot 'hydjet-cam-r0.225-cell0.00025.res' u 2:($4-$5*f($2)) w p lt 2pt 7 ps 2 t 'R = 0.225' 
`gnupr hydjet-scatter-cam-r0.225-corr.eps col`


set ylabel 'Jet P_t'
set yrange [-30:200]

plot 'hydjet-cam-r0.4-cell0.00025.res' u 2:($4) w p pt 7 ps 2 t 'uncorrected R = 0.4'
replot 'hydjet-cam-r0.4-cell0.00025.res' u 2:($4-$5*f($2)) w p pt 7 ps 2 t 'corrected R = 0.4'
`gnupr hydjet-scatter-cam-r0.4-both.eps col`


plot 'hydjet-cam-r0.3-cell0.00025.res' u 2:($4) w p pt 7 ps 2 t 'uncorrected R = 0.3'
replot 'hydjet-cam-r0.3-cell0.00025.res' u 2:($4-$5*f($2)) w p pt 7 ps 2 t 'corrected R = 0.3'
`gnupr hydjet-scatter-cam-r0.3-both.eps col`

plot 'hydjet-cam-r0.225-cell0.00025.res' u 2:($4) w p pt 7 ps 2 t 'uncorrected R = 0.225'
replot 'hydjet-cam-r0.225-cell0.00025.res' u 2:($4-$5*f($2)) w p pt 7 ps 2 t 'corrected R = 0.225'
`gnupr hydjet-scatter-cam-r0.225-both.eps col`
