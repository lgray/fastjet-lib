# gnuplot file

reset

# get a bunch of macros
load 'series.gp'

noUE='lt 4 lw 4 lc rgb "#e07e00"'
linenp ="lt  2 lw 4 lc rgb '#00a000'"
linenps="lt  1 lw 4"
linepu ="lt  3 lw 4"
linepus="lt -1 lw 2"
 
set ylabel '1/N dN/dm [GeV^{-1}]' offset 1.5,0
set xlabel 'reconstructed top mass [GeV]'

set key spacing 1.5

R=0.7

set xrange [150:200]
set yrange [0:0.035]

set ytics 0.01
set mxtics 2

set size 1.0,0.7

set label 1 "Cam/Aachen, R=0.7" at graph 0.025,0.90
set label 2 "Tevatron, {/Symbol \341}n_{PU}{/Symbol \361} = 2.3" at graph 0.025,0.07

plot cam(R,0) u 2:5 w histeps @noUE t 'no UE, no PU',\
     cam(R,1) u 2:5 w histeps @linenp t 'UE, no PU',\
     cam(R,1) u 2:13 w histeps @linepu t 'UE, PU'

`gnupr cam-tev-ttbar-paper-nosub.eps cld`

plot cam(R,0) u 2:5 w histeps @noUE t 'no UE, no PU',\
     cam(R,1) u 2:9 w histeps @linenps t 'UE, no PU (sub)',\
     cam(R,1) u 2:17 w histeps @linepus t 'UE, PU (sub)'

`gnupr cam-tev-ttbar-paper-sub.eps cld`
