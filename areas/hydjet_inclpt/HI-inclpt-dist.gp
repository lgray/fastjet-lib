# gnuplot file

reset

#set grid
set log y

# for getting grey of preliminary
unset colorbox
set palette gray

set xrange [-70:150]
set yrange [3e-6:1500]
set zrange [0:1]
set key spacing 1.5 width -4

set label 1 "Hydjet v 1.1"                          at 10,8e-5
set label 2 "[Pythia P_{t,min} = 10 GeV, quenched]" at 10,1e-5

set label 3 "k_t alg, R=0.4" at graph 0.03,0.93
set label 4 "|y| < 5" at graph 0.03,0.85
set label 5 "FastJet" at graph 0.03,0.77

set label 9 "PRELIMINARY" at graph 0.4,0.4 center rotate by 25 font "Helvetica,60" back tc palette cb 6

set xlabel 'P_t [GeV]'
set ylabel '1/n_{coll} d n_{jets} / d P_t'
#set format y "%.0t x 10^{%T}"
set format y "10^{%T}"


plot 'incplt_justhard_nhsel4_r0.4' u 1:($4*90) w st lt 2 lw 2 t 'scaled pp'
replot 'incplt_nhsel2_r0.4'        u 1:($7)    w st lt 3 lw 2 t 'raw Pb-Pb'
replot 'incplt_nhsel2_r0.4'        u 1:($9)    w st lt -1 lw 3 t 'Pb-Pb with subtraction'
