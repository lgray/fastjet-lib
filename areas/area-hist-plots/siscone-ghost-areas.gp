reset  

reset 
set sty dat li

set xrange [0.00:15]
set yrange [1e-3:10]
set log y
#set log x

set xlabel 'A / {/Symbol p}R^2'
set ylabel '{/Symbol p}R^2/N dN/dA ' offset 1

set key spacing 1.5
#set size square

#set label 1 'kt algorithm' at 1.9,2 right
#set label 2 '(a)' at graph 0.035,0.95
#plot 'cam-areas-long.res' t 'purely soft jets' w l lt 1 lw 3
#replot 'cam-areas-anchored-vlong.res' t 'jets with 1 hard parton' w l lt 3 lw 3


plot 'siscone-quickbigareas-kt10-f0.45-npass0-short.res' w histe lw 3 t 'f = 0.45'
replot 'siscone-quickbigareas-kt10-f0.50-npass0-short.res' w histe lw 3 t 'f = 0.50'
replot 'siscone-quickbigareas-kt10-f0.60-npass0-short.res' w histe lw 3 t 'f = 0.60'
replot 'siscone-quickbigareas-kt10-f0.75-npass0-short.res' w histe lw 3 t 'f = 0.75'

`gnupr siscone-ghost-areas.eps cld`
