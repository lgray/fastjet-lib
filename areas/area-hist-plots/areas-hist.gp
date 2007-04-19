reset 
set sty dat li

set xrange [0:2]

set xlabel 'A / {/Symbol p}R^2'
set ylabel '{/Symbol p}R^2/N dN/dA ' offset 1

set yrange [0:3]
set key spacing 1.5
set size square

set label 1 'kt algorithm' at 1.9,2 right
set label 2 '(a)' at graph 0.035,0.95
#plot 'cam-areas-long.res' t 'purely soft jets' w l lt 1 lw 3
#replot 'cam-areas-anchored-vlong.res' t 'jets with 1 hard parton' w l lt 3 lw 3

plot 'kt-areas-vlong.res'  u 1:2 w l lt 1 lw 3 t 'pure ghost jets'
`gnupr areas-hist-pure-soft.eps col`
replot 'kt-areas-anchored-widebin-vvlong.res'  u 1:2 w l lt 3 lw 3 t 'jets with 1 hard parton'
`gnupr areas-hist.eps cld`


set label 1 'Cam/Aachen algorithm'
set label 2 '(b)'
plot 'cam-areas-long.res'  u 1:2 w l lt 1 lw 3 t 'pure ghost jets'
replot 'cam-areas-anchored-widebin-vvlong.res'  u 1:2 w l lt 3 lw 3 t 'jets with 1 hard parton'
`gnupr areas-hist-cam.eps cld`

#replot 'kt-areas-vlong.res' t 'k_t purely soft jets'
#replot 'kt-areas-anchored-vlong.res' t 'k_t jets with 1 hard particle'
