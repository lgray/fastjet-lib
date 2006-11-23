reset 
set dat sty li

set xrange [0:2]

set xlabel 'A / {/Symbol p}R^2'
set ylabel '1/N dN/dA' 1

set yrange [0:3]
set key spacing 1.5
set size square

set label 1 'kt algorithm' at 1.9,2 right

#plot 'cam-areas-long.res' t 'purely soft jets' w l lt 1 lw 3
#replot 'cam-areas-anchored-vlong.res' t 'jets with 1 hard parton' w l lt 3 lw 3

plot 'kt-areas-vlong.res' t 'purely soft jets' w l lt 1 lw 3
replot 'kt-areas-anchored-vlong.res' t 'jets with 1 hard parton' w l lt 3 lw 3


#replot 'kt-areas-vlong.res' t 'k_t purely soft jets'
#replot 'kt-areas-anchored-vlong.res' t 'k_t jets with 1 hard particle'
