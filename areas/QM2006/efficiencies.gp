# gnuplot file

reset

set label 1 "dN_{ch}/dy = 1500" at graph 0.08,0.40
set label 2 "k_t R=0.4" at graph 0.08,0.32

set yrange [0:1.08]
set xrange [20:100]

set xlabel 'P_t [GeV]' 

set key bottom spacing 1.5

set size square
plot 1 lt 0 t ''

replot "<grep 'pt range' ../hydjet_eff/hydjetDEF_nhsel1_ptminhard10_kt_r0.4_maxrapphi0.2.res | sed 's/#.*range =//'" u ($1+0.01):(1-$13/($7)) w st lt 1 lw 2 t 'Efficiency',\
   '' u 1:(1-$10/($7+$10)) w st lt 3 lw 2 t 'Purity' 

#replot "<grep 'pt range' ../hydjet_eff/hydjetDEF_nhsel1_ptminhard10_kt_r0.4_maxrapphi0.2_discard1.0.res | sed 's/#.*range =//'" u ($1+0.01):(1-$13/($7)) w st lt 1 lw 2 t 'Efficiency',\
#   '' u 1:(1-$10/($7+$10)) w st lt 3 lw 2 t 'Purity' 