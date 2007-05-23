
reset
#set auto x
#set xrange [0:]

#plot "< cat sample-lhc-ptmin100-iseq05-kt-R0.7.res | grep -v '#' |  awk '{if ($2<4&&$2>-4) print $4,$5}'" u 2:1 w p  t ''

set label 1 "k_t algorithm, R=0.5" at graph 0.95,0.92 right
#set label 2 "R=0.5" at graph 0.95,0.87 right

set xlabel 'y_j'
set ylabel 'p_{tj} / A_j [GeV]' offset 1

set size 1,0.8
#set size 1.0
#set size ratio 0.8

set xrange [-4:4]
median=23.3123
err=6.14532
plot median+err with filledcurves above y1=(median-err) lt 6 t ''
replot median w l lt -1 t ''
#replot median-err w l lt -1 t ''
#replot median+err w l lt -1 t ''
replot "sample-lhc-ptmin100-iseq05-kt-R0.5.res" u 2:($4/$5) w p lt 1 pt 7 ps 2 t ''
