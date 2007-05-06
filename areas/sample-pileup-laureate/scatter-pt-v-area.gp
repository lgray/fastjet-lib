
set auto x
set xrange [0:]

set xlabel 'A_j'
set ylabel 'p_{tj} [GeV]' offset 1

median=23.3123
err=6.14532
mean_area=0.56316

set label 1 "k_t algorithm, R=0.5" at graph 0.05,0.92 left

set yrange [0:200]
set xrange [0:1.2]

plot median*x+err*sqrt(x*mean_area) with filledcurves above y1=0 lt 6 t ''
replot median*x-err*sqrt(x*mean_area) with filledcurves above y1=0 lt -2 t ''
replot median*x w l lt -1 t ''
replot "< cat sample-lhc-ptmin100-iseq05-kt-R0.5.res | grep -v '#' |  awk '{if ($2<4&&$2>-4) print $4,$5}'" u 2:1 w p lt 1 pt 7 ps 2 t ''

