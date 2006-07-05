# gnuplot file

reset
set xrange [-5:5]

# fit [-5:5] a+b*x**2 'HI-example-event.jets' u 2:($4/$5) via a,b
a=264.35
b=-6.177

# or with first two jets removed:
#a=255.7
#b=-5.61

set key spacing 1.5

# for getting grey of preliminary
unset colorbox
set palette gray

set xlabel 'rapidity'
set ylabel 'P_{t,jet} / Area_{jet}'

set label 3 "k_t alg, R=0.4" at graph 0.03,0.93
set label 4 "FastJet" at graph 0.03,0.85

set label 1 "Event from Hydjet v 1.1"               at graph 0.5,0.12 center
set label 2 "[Pythia P_{t,min} = 10 GeV, quenched, + 2x180GeV jets]" at graph 0.5,0.04 center

#set label 9 "PRELIMINARY" at graph 0.5,0.5 center rotate by 25 font "Helvetica,60" back tc palette cb 6


plot 'HI-example-event.jets' u 2:($4/$5) w p ps 2 pt 7 t ''
replot a+b*x**2  w l lt 3 lw 3 t 'fit of a+by^2'

