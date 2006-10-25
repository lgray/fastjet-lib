# gnuplot file

reset

#set grid
set log y

# for getting grey of preliminary
unset colorbox
set palette gray

set xrange [-70:250]
set yrange [3e-7:1500]
#set xrange [-70:450]
#set yrange [3e-9:1500]

set zrange [0:1]
set key spacing 1.5 width -4

justabove(f,x,xlim) = x >= xlim ? f : -1
justbelow(f,x,xlim) = x <= xlim ? f : -1

set label 1 "Hydjet v 1.1"                          at 12,8e-6
set label 2 "[Pythia P_{t,min} = 10 GeV, unquenched]" at 12,1e-6

set label 3 "k_t alg, R=0.4" at graph 0.03,0.93 front
set label 4 "|y| < 5" at graph 0.03,0.85
set label 5 "FastJet" at graph 0.03,0.77

set label 9 "PRELIMINARY" at graph 0.4,0.4 center rotate by 25 font "Helvetica,60" back tc palette cb 6

set xlabel 'P_t [GeV]'
set ylabel '1/n_{coll} d n_{jets} / d P_t'
#set format y "%.0t x 10^{%T}"
set format y "10^{%T}"


# rescaling factor for 
rescalehard=0.157
rescaleHI=0.135


plot 'incplt_justhard_nhsel3_r0.4' u 1:(justbelow($4*90,$1,92)) w st lw 2 lt 2 t 'scaled pp'
replot 'incplt_justhard_ptminhard50_nhsel3_r0.4' u 1:(justabove($4*rescalehard,$1,70)) w st lw 2 lt 2 t ''

#replot 'incplt_justhard_nhsel3_r0.4' u 2:(justbelow($4*90,$1,92))
#replot 'incplt_justhard_ptminhard50_nhsel3_r0.4' u 2:(justabove($4*rescalehard,$1,70))

replot 'incplt_xcl_nhsel1_r0.4'        u 1:($7)    w st lw 2 lt 3 t 'raw Pb-Pb'
#replot 'incplt_nhsel1_r0.4'        u 1:($9)    w st lw 2 lt 55 t 'Pb-Pb with subtraction'
replot 'incplt_xcl_nhsel1_r0.4'        u 1:(justbelow($9,$1,140))    w st lw 3 lt -1 t 'Pb-Pb with subtraction'
replot 'incplt_xcl_ptminhard50_nhsel1_r0.4'        u 1:(justabove($9*rescaleHI,$1,100))    w st lw 3 lt -1 t ''

#replot 'incplt_xcl_ptminhard50_nhsel1_r0.4'        u 1:(justabove($9*rescaleHI,$1,80))    w st lw 2 lt 4 t ''
#replot 'incplt_xcl_ptminhard50_nhsel1_r0.4'        u 1:(justabove($11*rescaleHI,$1,80))    w st lw 2 lt 5 t ''

#replot 'incplt_ptminhard50_nhsel1_r0.4'        u 1:(justabove($8*rescaleHI,$1,80))    w st lw 2 lt 5 t ''

# assume 120 soft jets, 2 hard,  and effect of hard jets on mean subtraction
# is E_{t,hard} * n_{hard} / n_soft...
#replot 'incplt_ptminhard50_nhsel1_r0.4'        u ($1+$1/60):(justabove($9*rescaleHI,$1,80))    w st lw 1 lt 5 t ''


# # attempt with r=0.7 -- tbc
#replot 'incplt_justhard_ptminhard50_nhsel3_r0.7'  u 1:(justabove($4*rescalehard,$1,70)) w st lw 2 lt 4 t ''
#replot 'incplt_ptminhard50_nhsel1_r0.7'  u 1:(justabove($9*rescaleHI,$1,80))    w st lw 2 lt 5 t ''
