# gnuplot file

reset 
set dat sty st

set size square
set xlabel 'Jet Area (A) / {/Symbol p} R^2'
set ylabel '1/N dN/dA' 0.9

set xrange [0:1.8]
set yrange [0:2]

set label 1 'Fake jets' at 0.63,1.9 
set label 2 'Jets with' at 1.15,1.1 
set label 3 'P_t > 100 GeV' at 1.15,0.97

set xtics 0,0.5

plot '../hydjet_eff/hydjetDEF_nhsel1_ptminhard50_kt_r0.4_maxrapphi0.2.NEW.res' i 36 u ($1-0.005):($4*10) w st lt 1 lw 3 t ''
replot '../hydjet_eff/hydjetDEF_nhsel1_ptminhard50_kt_r0.4_maxrapphi0.2.NEW.res' i 35 u ($1+0.005):($4*10) w st lt 3 lw 3 t ''

